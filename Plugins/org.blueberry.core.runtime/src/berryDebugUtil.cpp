/*===================================================================

BlueBerry Platform

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "berryIDebugObjectListener.h"

#include "berryDebugUtil.h"
#include "berryObject.h"
#include "berryLog.h"
#include "berryPlatform.h"
#include "berryDebugBreakpointManager.h"

#include "internal/berryCTKPluginActivator.h"

#include <QDir>
#include <QDebug>

#include <Poco/Bugcheck.h>
#include <Poco/NumberParser.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/SAX/SAXException.h>

#include <Poco/FileStream.h>

#include <sstream>

#include <numeric>

namespace berry
{

static IDebugObjectListener::Events _G_ObjectEvents;

const QString DebugUtil::DEBUG_UTIL_XML = "debugutil.xml";
const QString DebugUtil::DEBUGUTIL_TAG = "debugutil";
const QString DebugUtil::TRACEOBJECT_TAG = "traceObject";
const QString DebugUtil::TRACECLASS_TAG = "traceClass";
const QString DebugUtil::ID_ATTR = "id";
const QString DebugUtil::NAME_ATTR = "name";

class NotClassName: public std::unary_function<const Object*, bool>
{

  QString name;

public:
  NotClassName(const QString& s) :
    name(s)
  {
  }
  bool operator()(const Object* entry) const
  {
    return name != entry->GetClassName();
  }
};

class AccumulateClassNames: public std::binary_function<QSet<QString>*, const Object*, QSet<QString>*>
{
public:
  QSet<QString>* operator()(QSet<QString>* names, const Object* entry)
  {
    names->insert(entry->GetClassName());
    return names;
  }
};

DebugBreakpointManager* DebugUtil::GetBreakpointManager()
{
  static DebugBreakpointManager* breakpointManager = new DebugBreakpointManager();
  return breakpointManager;
}

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::TraceObject(const Object* object)
{
  BERRY_INFO << "Tracing enabled for: " << object->GetTraceId() << std::endl;
  GetTracedObjects()->insert(object->GetTraceId());
  _G_ObjectEvents.objTracingEvent(object->GetTraceId(), true, object);
}
#else
void DebugUtil::TraceObject(const Object*  /*object*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::TraceObject(unsigned int traceId)
{
  BERRY_INFO << "Tracing enabled for: " << traceId << std::endl;
  GetTracedObjects()->insert(traceId);
  TraceIdToObjectType::ConstIterator i = GetTraceIdToObjectMap()->find(traceId);
  if (i != GetTraceIdToObjectMap()->end())
    _G_ObjectEvents.objTracingEvent(traceId, true, i.value());
  else
    _G_ObjectEvents.objTracingEvent(traceId, true, 0);
}
#else
void DebugUtil::TraceObject(unsigned int /*traceId*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::TraceClass(const QString& className)
{
  BERRY_INFO << "Tracing enabled for: " << className << std::endl;
  GetTracedClasses()->insert(className);
  //_G_ObjectEvents.objTracingEvent(object->GetTraceId(), true, object);
}
#else
void DebugUtil::TraceClass(const QString&  /*className*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::StopTracing(unsigned int traceId)
{

  BERRY_INFO << "Tracing stopped for: " << traceId << std::endl;
  GetTracedObjects()->remove(traceId);
  TraceIdToObjectType::ConstIterator i = GetTraceIdToObjectMap()->find(traceId);
  if (i != GetTraceIdToObjectMap()->end())
    _G_ObjectEvents.objTracingEvent(traceId, false, i.value());
  else
    _G_ObjectEvents.objTracingEvent(traceId, false, 0);
}
#else
void DebugUtil::StopTracing(unsigned int /*traceId*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::StopTracing(const Object* obj)
{
  BERRY_INFO << "Tracing stopped for: " << obj->GetTraceId() << std::endl;
  GetTracedObjects()->remove(obj->GetTraceId());
  _G_ObjectEvents.objTracingEvent(obj->GetTraceId(), false, obj);
}
#else
void DebugUtil::StopTracing(const Object* /*obj*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::StopTracing(const QString& className)
{
  BERRY_INFO << "Tracing stopped for: " << className << std::endl;
  GetTracedClasses()->remove(className);
  //_G_ObjectEvents.objTracingEvent(obj->GetTraceId(), false, obj);
}
#else
void DebugUtil::StopTracing(const QString& /*className*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
bool DebugUtil::IsTraced(const Object* object)
{
  if (GetTracedObjects()->find(object->GetTraceId()) != GetTracedObjects()->end())
    return true;

  if (GetTracedClasses()->find(object->GetClassName()) != GetTracedClasses()->end())
      return true;

  return false;
}
#else
bool DebugUtil::IsTraced(const Object*  /*object*/)
{
  return false;
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
bool DebugUtil::IsTraced(unsigned int traceId)
{
  if (GetTracedObjects()->find(traceId) != GetTracedObjects()->end())
    return true;

  TraceIdToObjectType::Iterator it = GetTraceIdToObjectMap()->find(traceId);
  if (it != GetTraceIdToObjectMap()->end())
  {
    if (GetTracedClasses()->find(it.value()->GetClassName()) != GetTracedClasses()->end())
      return true;
  }

  return false;
}
#else
bool DebugUtil::IsTraced(unsigned int /*traceId*/)
{
  return false;
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
bool DebugUtil::IsTraced(const QString& className)
{
  return GetTracedClasses()->find(className) != GetTracedClasses()->end();
}
#else
bool DebugUtil::IsTraced(const QString&  /*className*/)
{
  return false;
}
#endif


const Object* DebugUtil::GetObject(unsigned int traceId)
{
  return (*GetTraceIdToObjectMap())[traceId];
}

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
QList<unsigned int> DebugUtil::GetSmartPointerIDs(
    const Object* objectPointer, const QList<unsigned int>& excludeList)
{
  Q_ASSERT(objectPointer != 0);
  QList<unsigned int> ids = (*GetTraceIdToSmartPointerMap())[objectPointer->GetTraceId()];
  for (QList<unsigned int>::const_iterator iter = excludeList.begin();
      iter != excludeList.end(); ++iter)
  ids.removeAll(*iter);
  return ids;
}
#else
QList<unsigned int> DebugUtil::GetSmartPointerIDs(
    const Object* /*objectPointer*/, const QList<unsigned int>&  /*excludeList*/)
{
  return QList<unsigned int>();
}
#endif

QList<const Object*> DebugUtil::GetRegisteredObjects()
{
  return GetTraceIdToObjectMap()->values();
}

void DebugUtil::PrintSmartPointerIDs(const Object* objectPointer, const QList<unsigned int>& excludeList)
{
  qDebug() << "SmartPointer IDs [ ";
  if (IsTraced(objectPointer))
  {
    QList<unsigned int> ids = GetSmartPointerIDs(objectPointer, excludeList);
    for (QList<unsigned int>::const_iterator iter = ids.begin();
        iter != ids.end(); ++iter)
    {
      qDebug() << *iter << " ";
    }
  }
  else
  {
    qDebug() << "n/a ";
  }
  qDebug() << "]\n";
}

void DebugUtil::AddObjectListener(IDebugObjectListener* listener)
{
  _G_ObjectEvents.AddListener(listener);
}

void DebugUtil::RemoveObjectListener(IDebugObjectListener* listener)
{
  _G_ObjectEvents.RemoveListener(listener);
}

void DebugUtil::ResetObjectSummary()
{
  GetTraceIdToObjectMap()->clear();
  GetTraceIdToSmartPointerMap()->clear();
  GetTracedObjects()->clear();
}

bool DebugUtil::PrintObjectSummary(bool details)
{
  QSet<QString> names;
  std::accumulate(GetTraceIdToObjectMap()->begin(), GetTraceIdToObjectMap()->end(), &names, AccumulateClassNames());

  if (!names.isEmpty())
  {
    std::cout << std::endl << std::endl << "#########################################################" << std::endl;
    std::cout << "########     berry::Object leakage summary:     ########" << std::endl << std::endl;

    for (QSet<QString>::const_iterator i = names.begin();
         i != names.end(); ++i)
    {
      PrintObjectSummary(*i, details);
      if (details) std::cout << std::endl;
    }

    std::cout << std::endl << "#########################################################" << std::endl << std::endl;
  }

  return !names.isEmpty();
}

bool DebugUtil::PrintObjectSummary(const QString& className, bool details)
{
  TraceIdToObjectType::ConstIterator endIter =
  std::remove_if(GetTraceIdToObjectMap()->begin(), GetTraceIdToObjectMap()->end(), NotClassName(className));

  qDebug() << "Class:" << className;
  if (details) std::cout << std::endl;

  std::size_t count = 0;
  for (TraceIdToObjectType::ConstIterator iter = GetTraceIdToObjectMap()->begin();
      iter != endIter; ++iter, ++count)
  {
    if (details)
    {
      qDebug() << (*(iter.value()));
      PrintSmartPointerIDs(iter.value());
    }
  }
  qDebug() << "(" << count << " instances)\n";
  return (count!=0);
}

unsigned int& DebugUtil::GetSmartPointerCounter()
{
  static unsigned int counter = 0;
  return counter;
}

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::UnregisterSmartPointer(unsigned int smartPointerId, const Object* objectPointer)
{
  poco_assert(objectPointer != 0);

  (*GetTraceIdToSmartPointerMap())[objectPointer->GetTraceId()].removeAll(smartPointerId);
  _G_ObjectEvents.spDestroyedEvent(smartPointerId, objectPointer);
}
#else
void DebugUtil::UnregisterSmartPointer(unsigned int  /*smartPointerId*/, const Object*  /*objectPointer*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::RegisterSmartPointer(unsigned int  smartPointerId, const Object* objectPointer, bool /*recordStack*/)
{
  poco_assert(objectPointer != 0);

  if (GetTracedClasses()->find(objectPointer->GetClassName()) != GetTracedClasses()->end() ||
      GetTracedObjects()->find(objectPointer->GetTraceId()) != GetTracedObjects()->end())
  {
    (*GetTraceIdToSmartPointerMap())[objectPointer->GetTraceId()].push_back(smartPointerId);
    _G_ObjectEvents.spCreatedEvent(smartPointerId, objectPointer);
  }

  if (GetBreakpointManager()->BreakAtSmartpointer(smartPointerId))
  poco_debugger_msg("SmartPointer Breakpoint reached");
}
#else
void DebugUtil::RegisterSmartPointer(unsigned int  /*smartPointerId*/, const Object*  /*objectPointer*/, bool /*recordStack*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::RegisterObject(const Object* objectPointer)
{
  GetTraceIdToObjectMap()->insert(objectPointer->GetTraceId(), objectPointer);
  _G_ObjectEvents.objCreatedEvent(objectPointer);

  if (GetBreakpointManager()->BreakAtObject(objectPointer->GetTraceId()))
  {
    std::string msg = "SmartPointer Breakpoint reached for ";
    msg += objectPointer->GetClassName().toStdString();
    poco_debugger_msg(msg.c_str());
  }
}
#else
void DebugUtil::RegisterObject(const Object* /*objectPointer*/)
{
}
#endif

#ifdef BLUEBERRY_DEBUG_SMARTPOINTER
void DebugUtil::UnregisterObject(const Object* objectPointer)
{
  GetTraceIdToObjectMap()->remove(objectPointer->GetTraceId());
  _G_ObjectEvents.objDestroyedEvent(objectPointer);
}
#else
void DebugUtil::UnregisterObject(const Object* /*objectPointer*/)
{
}
#endif

bool DebugUtil::GetPersistencePath(QDir& path)
{
  QFileInfo statePath = CTKPluginActivator::getPluginContext()->getDataFile(QString());
  path = statePath.absoluteFilePath();
  return true;
}

void DebugUtil::SaveState(const QDir& path)
{
  QString saveFile = path.absoluteFilePath(DEBUG_UTIL_XML);

  auto   doc = new Poco::XML::Document();
  Poco::XML::Element* debugutil = doc->createElement(DEBUGUTIL_TAG.toStdString());
  doc->appendChild(debugutil)->release();

  for (QSet<unsigned int>::const_iterator i = GetTracedObjects()->begin();
       i != GetTracedObjects()->end(); ++i)
  {
    Poco::XML::Element* traceObject = doc->createElement(TRACEOBJECT_TAG.toStdString());
    debugutil->appendChild(traceObject)->release();
    traceObject->setAttribute(ID_ATTR.toStdString(), QString::number(*i).toStdString());
  }

  for (QSet<QString>::const_iterator i = GetTracedClasses()->begin();
       i != GetTracedClasses()->end(); ++i)
  {
    Poco::XML::Element* traceClass = doc->createElement(TRACECLASS_TAG.toStdString());
    debugutil->appendChild(traceClass)->release();
    traceClass->setAttribute(NAME_ATTR.toStdString(), i->toStdString());
  }

  try
  {
    Poco::FileOutputStream writer(saveFile.toStdString());
    Poco::XML::DOMWriter out;
    out.setOptions(3); //write declaration and pretty print
    out.writeNode(writer, doc);

    doc->release();

    // save BreakpointManager
    QString saveBM = path.absoluteFilePath(QString::fromStdString(DebugBreakpointManager::BREAKPOINTS_XML));
    GetBreakpointManager()->SaveState(saveBM);
  }
  catch (Poco::FileException& e)
  {
    BERRY_WARN << e.displayText();
  }

}

void DebugUtil::RestoreState(const QDir& path)
{
  QString restoreFile = path.absoluteFilePath(DEBUG_UTIL_XML);

  try
  {
    Poco::XML::DOMParser parser;

    Poco::FileInputStream reader(restoreFile.toStdString());
    Poco::XML::InputSource source(reader);

    //source.setSystemId(baseDir);
    Poco::XML::Document* doc = parser.parse(&source);
    Poco::XML::Element* debugutil = doc->documentElement();

    if (debugutil)
    {
      // restore traced objects
      Poco::XML::NodeList* elementList = debugutil->getElementsByTagName(TRACEOBJECT_TAG.toStdString());
      for (std::size_t i = 0; i < elementList->length(); i++)
      {
        Poco::XML::Element* elem =
        dynamic_cast<Poco::XML::Element*> (elementList->item(static_cast<unsigned long>(i)));

        if (!elem->hasAttribute(ID_ATTR.toStdString())) continue;

        const std::string& attr = elem->getAttribute(ID_ATTR.toStdString());

        int traceId = 0;
        try
        {
          traceId = Poco::NumberParser::parse(attr);
        }
        catch (const Poco::SyntaxException& e)
        {
          BERRY_WARN << e.displayText();
        }

        DebugUtil::TraceObject(traceId);
      }
      elementList->release();

      // restore traced classes
      elementList = debugutil->getElementsByTagName(TRACECLASS_TAG.toStdString());
      for (std::size_t i = 0; i < elementList->length(); i++)
      {
        Poco::XML::Element* elem =
        dynamic_cast<Poco::XML::Element*> (elementList->item(static_cast<unsigned long>(i)));

        if (!elem->hasAttribute(NAME_ATTR.toStdString())) continue;

        const std::string& traceClass = elem->getAttribute(NAME_ATTR.toStdString());
        if (!traceClass.empty())
          DebugUtil::TraceClass(QString::fromStdString(traceClass));
      }
      elementList->release();
    }

    doc->release();
  }
  catch (Poco::XML::SAXParseException& e)
  {
    BERRY_WARN << e.displayText();
  }
  catch (Poco::FileNotFoundException&)
  {

  }
  catch (Poco::FileException& e)
  {
    BERRY_WARN << e.displayText();
  }

  // restore BreakpointManager
  GetBreakpointManager()->RestoreState(path.absoluteFilePath(QString::fromStdString(DebugBreakpointManager::BREAKPOINTS_XML)));
}

}

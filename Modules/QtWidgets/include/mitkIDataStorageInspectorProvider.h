/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef __I_DATA_STORAGE_INSPECTOR_PROVIDER_H
#define __I_DATA_STORAGE_INSPECTOR_PROVIDER_H

#include <mitkServiceInterface.h>

#include <MitkQtWidgetsExports.h>

class QmitkAbstractDataStorageInspector;

namespace mitk
{
  /**
   * \ingroup MicroServices_Interfaces
   *
   * \brief The common interface for all DataStorage inspector providers.
   *
   * Implementations of this interface must be registered as a service
   * to make themselves available via the service registry.
   *
   * It is recommended to derive new implementations from QmitkDataStorageInspectorProviderBase
   * which provide correct service registration semantics.
   *
   * \sa QmitkDataStorageInspectorProviderBase
   */
  struct MITKQTWIDGETS_EXPORT IDataStorageInspectorProvider
  {
    virtual ~IDataStorageInspectorProvider();

    /**
     * \brief returns an inspector instance represented by the provider.
     */
    virtual QmitkAbstractDataStorageInspector* CreateInspector() const = 0;

    /** Return the uniqe ID for the inspector type provided.*/
    virtual std::string GetInspectorID() const = 0;
    /** Return the display name (e.g. used in the UI) for the inspector type provided.*/
    virtual std::string GetInspectorDisplayName() const = 0;
    /** Returns a description of the inspector type provided.*/
    virtual std::string GetInspectorDescription() const = 0;

    /**
    * @brief Service property name for the inspector ID.
    *
    * The property value must be of type \c std::string.
    *
    * @return The property name.
    */
    static std::string PROP_INSPECTOR_ID();
  };

} // namespace mitk

MITK_DECLARE_SERVICE_INTERFACE(mitk::IDataStorageInspectorProvider, "org.mitk.IDataStorageInspectorProvider")

#endif /* __I_DATA_STORAGE_INSPECTOR_PROVIDER_H */

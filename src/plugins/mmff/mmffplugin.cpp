/******************************************************************************
**
** Copyright (C) 2009-2011 Kyle Lutz <kyle.r.lutz@gmail.com>
**
** This file is part of chemkit. For more information see
** <http://www.chemkit.org>.
**
** chemkit is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** chemkit is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with chemkit. If not, see <http://www.gnu.org/licenses/>.
**
******************************************************************************/

#include "mmffplugin.h"

#include "mmffforcefield.h"
#include "mmffparametersdata.h"

MmffPlugin::MmffPlugin()
    : chemkit::Plugin("mmff")
{
    chemkit::ForceField::registerForceField("mmff", &createMmffForceField);
}

MmffPlugin::~MmffPlugin()
{
    foreach(MmffParametersData *parameters, m_parametersCache.values()){
        parameters->deref();
    }
}

void MmffPlugin::storeParameters(const QString &name, MmffParametersData *parameters)
{
    if(m_parametersCache.contains(name)){
        m_parametersCache[name]->deref();
    }

    m_parametersCache.insert(name, parameters);
    parameters->ref();
}

MmffParametersData* MmffPlugin::parameters(const QString &name) const
{
    return m_parametersCache.value(name, 0);
}

chemkit::ForceField* MmffPlugin::createMmffForceField()
{
    return new MmffForceField;
}

Q_EXPORT_PLUGIN2(mmff, MmffPlugin);

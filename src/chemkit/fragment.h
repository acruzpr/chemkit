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

#ifndef CHEMKIT_FRAGMENT_H
#define CHEMKIT_FRAGMENT_H

#include "chemkit.h"

#include <vector>

#include <QtCore>

namespace chemkit {

class Atom;
class Bond;
class Molecule;

class CHEMKIT_EXPORT Fragment
{
    public:
        // properties
        int size() const;
        Molecule* molecule() const;

        // structure
        Atom* atom(int index) const;
        std::vector<Atom *> atoms() const;
        int atomCount() const;
        bool contains(const Atom *atom) const;
        std::vector<Bond *> bonds() const;
        int bondCount() const;
        bool contains(const Bond *bond) const;

    private:
        Fragment(Atom *root);
        ~Fragment();

        Q_DISABLE_COPY(Fragment)

        friend class Molecule;

    private:
        std::vector<Atom *> m_atoms;
};

} // end chemkit namespace

#include "fragment-inline.h"

#endif // CHEMKIT_FRAGMENT_H

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2018-2022 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::coupledInstant

Description
    An instant of time. Contains the time value and name.
    Uses Foam::Time when formatting the name.

SourceFiles
    coupledInstants.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_coupled_instant_H
#define Foam_coupled_instant_H

#include "CoupledInstant.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class coupled instant Declaration
\*---------------------------------------------------------------------------*/

class coupledInstant
:
    public CoupledInstant<word>
{
public:

    // Static Data Members

        //- The type name (eg, for pTraits)
        static const char* const typeName;


    // Generated Methods

        //- Default construct, with value = 0 and empty name
        coupledInstant() = default;

        //- Copy construct
        coupledInstant(const coupledInstant&) = default;

        //- Move construct
        coupledInstant(coupledInstant&&) = default;

        //- Copy assignment
        coupledInstant& operator=(const coupledInstant&) = default;

        //- Move assignment
        coupledInstant& operator=(coupledInstant&&) = default;


    // Constructors

        //- Inherit all constructors (eg, copy/move construct from components)
        using CoupledInstant<word>::CoupledInstant;

        //- Construct from timeValue, auto generating the name
        explicit coupledInstant(scalar timeValue);

        //- Copy construct from timeName, parsing timeName for a value
        explicit coupledInstant(const word& timeName);

        //- Move construct from timeName, parsing timeName for a value
        explicit coupledInstant(word&& timeName);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

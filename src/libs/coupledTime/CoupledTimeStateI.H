/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
    Copyright (C) 2021 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline Foam::scalar Foam::CoupledTimeState::timeOutputValue() const
{
    return timeToUserTime(value());
}


inline Foam::label Foam::CoupledTimeState::timeIndex() const noexcept
{
    return timeIndex_;
}


inline Foam::scalar Foam::CoupledTimeState::deltaTValue() const noexcept
{
    return deltaT_;
}


inline Foam::scalar Foam::CoupledTimeState::deltaT0Value() const noexcept
{
    return deltaT0_;
}


inline Foam::dimensionedScalar Foam::CoupledTimeState::deltaT() const
{
    return dimensionedScalar("deltaT", dimTime, deltaT_);
}


inline Foam::dimensionedScalar Foam::CoupledTimeState::deltaT0() const
{
    return dimensionedScalar("deltaT0", dimTime, deltaT0_);
}


inline bool Foam::CoupledTimeState::writeTime() const noexcept
{
    return writeTime_;
}


// ************************************************************************* //

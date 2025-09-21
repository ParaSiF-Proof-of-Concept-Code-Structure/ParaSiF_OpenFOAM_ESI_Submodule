/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 OpenCFD Ltd.
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

#include "subLoopCoupledTime.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::subLoopCoupledTime::subLoopCoupledTime(CoupledTime& runTime, const label nCycles)
:
    time_(runTime),
    index_(0),
    total_(nCycles)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::subLoopCoupledTime::~subLoopCoupledTime()
{
    stop();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::subLoopCoupledTime::status() const
{
    return (index_ < total_);
}


void Foam::subLoopCoupledTime::stop()
{
    // If called manually, ensure status() will return false
    index_ = total_ + 1;
}


bool Foam::subLoopCoupledTime::loop()
{
    const bool active = (index_ < total_);   // as per status()

    if (active)
    {
        operator++();
    }
    else if (index_)
    {
        // Not active, the loop condition has now exiting on the last subloop
    }

    return active;
}


Foam::subLoopCoupledTime& Foam::subLoopCoupledTime::operator++()
{
    ++index_;
    return *this;
}


// ************************************************************************* //

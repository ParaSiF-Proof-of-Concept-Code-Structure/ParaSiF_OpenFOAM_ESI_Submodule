/*****************************************************************************
* Parallel Partitioned Multi-Physics Simulation Framework (ParaSiF)          *
*                                                                            *
* Copyright (C) 2025 The ParaSiF Development Team                            *
* All rights reserved                                                        *
*                                                                            *
* This software is licensed under the GNU General Public License version 3   *
*                                                                            *
* ** GNU General Public License, version 3 **                                *
*                                                                            *
* This program is free software: you can redistribute it and/or modify       *
* it under the terms of the GNU General Public License as published by       *
* the Free Software Foundation, either version 3 of the License, or          *
* (at your option) any later version.                                        *
*                                                                            *
* This program is distributed in the hope that it will be useful,            *
* but WITHOUT ANY WARRANTY; without even the implied warranty of             *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
* GNU General Public License for more details.                               *
*                                                                            *
* You should have received a copy of the GNU General Public License          *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
*****************************************************************************/

string fsiFile("fsiDict");
string fsiForceName("coupledForces");
dictionary fsiForceDict ;
word couplingMode;
IOdictionary fsiDict
(
    IOobject
    (
        fsiFile,
        runTime.system(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);


    // Define the name of MUI interfaceName
    List<word> interfaceNames;
    List<List<word>> patchNames;

    List<List<word>> patch6DoFNames;
    double forgetTime = 1.0;

    word sendNameX;
    word sendNameY;
    word sendNameZ;

    word fetchNameX;
    word fetchNameY;
    word fetchNameZ;

    double rSampler;

    //- Tight/Loose coupling method
    word cplMethod;

    //- Initial under relaxation factor of the coupling
    double initUndRelxCpl;

    //- Whether to push force or traction
    bool pushForce;

    //- Whether apply the fetched X value or not(i.e. one-way coupling)
    bool applyFetchX;
    //- Whether apply the fetched Y value or not(i.e. one-way coupling)
    bool applyFetchY;
    //- Whether apply the fetched Z value or not(i.e. one-way coupling)
    bool applyFetchZ;

    //- Number of time steps that will not push forces/tractions through MUI.
    double pushDelay;

    //- Whether it is a 2D case, i.e. whether to have a 2D force correction before push?
    //- A 2D case is the case that the front and back boundaries are empty boundary condition.
    //- There usually has one cell between the front and back boundaries.
    bool twoDimCorrection;

    //- The cell height of the only cell between the two empty boundaries [m].
    //- Valid only when twoDimCorrection is true.
    double twoDimCellHeight;

    scalar rigidMotionFrqX_;
    scalar rigidMotionFrqY_;
    scalar rigidMotionFrqZ_;

    scalar rigidMotionAmplitudeX_;
    scalar rigidMotionAmplitudeY_;
    scalar rigidMotionAmplitudeZ_;

    scalar rotation2DFrq_ = 0.0;
    scalar rotation2DAmplitude_ = 0.0;
    scalar rotation2DOriginX_ = 0.0;
    scalar rotation2DOriginY_ = 0.0;
    scalar rotation2DOriginZ_ = 0.0;

    List<List<int>> patchIDs;
    List<List<int>> patch6DoFIDs;
    

if (fsiDict.headerOk())
{

    if (!(fsiDict.readIfPresent("couplingMode", couplingMode)))
    {
        FatalIOErrorIn("", fsiDict)
                        << "Missing couplingMode entry" << exit(FatalIOError);
    }


    if (couplingMode=="singlePoint"){
        fsiForceDict = fsiDict.subDict(fsiForceName);
    }else if(couplingMode=="boundaryPatch"){
        #include "readFSIDict.H"
    } else {
        FatalIOErrorIn("", fsiDict)
          << "The selected couplingMode entry is invalid. The valid options are singlePoint and boundaryPatch" << exit(FatalIOError);
    }
    
    // word cplFSIMethod;
    // if (!(fsiDict.readIfPresent("cplMethod", cplFSIMethod)))
    // {
    //     FatalIOErrorIn("", fsiDict)
    //                     << "Missing cplMethod entry" << exit(FatalIOError);
    // }
    // cplMethod = cplFSIMethod;

    
    // if (cplMethod != "Loose")
    // {
    //     double initFSIUndRelxCpl;
    //     if (!(fsiDict.readIfPresent("initUndRelxCpl", initFSIUndRelxCpl)))
    //     {
    //         FatalIOErrorIn("", fsiDict)
    //                         << "Missing initUndRelxCpl entry" << exit(FatalIOError);
    //     }
    //     initUndRelxCpl = initFSIUndRelxCpl;
    // }

} else {
    FatalIOErrorIn("", fsiDict)
            << "Missing fsiDict" << exit(FatalIOError);
}

    // Define the name of MUI interfaces
    std::vector<std::string> interfaces;

    forAll(interfaceNames, iN)
    {
        interfaces.emplace_back(interfaceNames[iN]);
    }
    // Declare MUI objects using MUI configure file

    auto& ifs = runTime.mui_ifs;

    scalar sendXMin = VGREAT;
    scalar sendYMin = VGREAT;
    scalar sendZMin = VGREAT;

    scalar sendXMax = -VGREAT;
    scalar sendYMax = -VGREAT;
    scalar sendZMax = -VGREAT;

volVectorField * faceForces = new volVectorField
    (
        IOobject
        (
            "faceForces",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedVector
        (
            "faceForces",
            dimensionSet(1,1,-2,0,0,0,0),
            vector::zero
        )
    );

    volVectorField * faceForces2D = new volVectorField
    (
        IOobject
        (
            "faceForces2D",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedVector
        (
            "faceForces2D",
            dimensionSet(1,1,-2,0,0,0,0),
            vector::zero
        )
    );

    volVectorField * faceTraction = new volVectorField
    (
        IOobject
        (
            "faceTraction",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedVector
        (
            "faceTraction",
            dimensionSet(1,-1,-2,0,0,0,0),
            vector::zero
        )
    );

    List<List<vectorField>> oriCoordPushPointsVectorValsList;
    bool rankHasPatchPointsForPush = false;
    bool rankHasPatchPointsForfetch = false;
    List<scalar> totalFaceNList;


    List<List<vectorField>> dispVectorValsList;
    List<List<vectorField>> dispVectorOldValsList;
    List<List<vectorField>> oriCoordFetchPointsVectorValsList;

    
    // define spatial and temporal samplers
    // mui::sampler_pseudo_n2_linear<mui::mui_config>* spatial_sampler = nullptr;
    // mui::sampler_exact<mui::mui_config> spatial_sampler;
    // mui::temporal_sampler_exact<mui::mui_config> chrono_sampler;

	mui::algo_aitken<mui::mui_config>* aitken_x = nullptr;
    mui::algo_aitken<mui::mui_config>* aitken_y = nullptr;
    mui::algo_aitken<mui::mui_config>* aitken_z = nullptr;

	mui::algo_fixed_relaxation<mui::mui_config>* fr_x = nullptr;
    mui::algo_fixed_relaxation<mui::mui_config>* fr_y = nullptr;
    mui::algo_fixed_relaxation<mui::mui_config>* fr_z = nullptr;

//////////////////////////////////
    mui::sampler_pseudo_n2_linear<mui::mui_config> spatial_sampler(rSampler);
    // mui::sampler_exact<mui::mui_config> spatial_sampler;
    mui::temporal_sampler_exact<mui::mui_config> chrono_sampler;

	// mui::algo_aitken<mui::mui_config> aitken_x(static_cast<double>(initUndRelxCpl),1.0,ptsVluInit);
    // mui::algo_aitken<mui::mui_config> aitken_y(static_cast<double>(initUndRelxCpl),1.0,ptsVluInit);
    // mui::algo_aitken<mui::mui_config> aitken_z(static_cast<double>(initUndRelxCpl),1.0,ptsVluInit);

	// mui::algo_fixed_relaxation<mui::mui_config> fr_x(static_cast<double>(initUndRelxCpl),ptsVluInit);
    // mui::algo_fixed_relaxation<mui::mui_config> fr_y(static_cast<double>(initUndRelxCpl),ptsVluInit);
    // mui::algo_fixed_relaxation<mui::mui_config> fr_z(static_cast<double>(initUndRelxCpl),ptsVluInit);


    
    fileName muiResidualFile("muiFSIResidual.txt");
    OFstream muiResidual(runTime.system()/muiResidualFile);
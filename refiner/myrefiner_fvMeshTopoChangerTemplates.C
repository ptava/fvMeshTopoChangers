/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2025 OpenFOAM Foundation
     \\/     M anipulation  |
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
#include "surfaceFields.H"
#include "volFields.H"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class T>
inline void Foam::fvMeshTopoChangers::myrefiner::setInfo
(
    const word& name,
    const T& value
) const
{
    if (mesh().foundObject<uniformDimensionedScalarField>(name))
    {
        auto& fld = const_cast<uniformDimensionedScalarField&>(
            mesh().lookupObject<uniformDimensionedScalarField>(name)
        );
        fld.value() = value;
    }
    else
    {
        mesh().thisDb().store
        (
            new uniformDimensionedScalarField
            (
                IOobject
                (
                    name,
                    mesh().time().time().name(),
                    mesh().thisDb(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                dimensionedScalar(name, dimless, value)
            )
        );
    }
}

template<Foam::autoPtr<Foam::volScalarField> Foam::fvMeshTopoChangers::myrefiner::* MemberPtr>
inline void Foam::fvMeshTopoChangers::myrefiner::setDumpField
(
    const scalarField& vals,
    const word& name
) const
{
    // Sanity check
    if (vals.size() != mesh().nCells())
    {
        FatalErrorInFunction
            << "Size mismatch for field '" << name << "': vals.size() = "
            << vals.size() << " vs nCells() = " << mesh().nCells()
            << exit(FatalError);
    }

    // Resolve member pointer to actual autoPtr
    autoPtr<volScalarField>& fld = const_cast<autoPtr<volScalarField>&>(
        this->*MemberPtr
    );

    // Ensure field exists and matches current mesh
    makeDumpField(fld, name);

    // Assign internal values
    fld->primitiveFieldRef() = vals;
}

template<Foam::autoPtr<Foam::labelIOList> Foam::fvMeshTopoChangers::myrefiner::* MemberPtr>
inline void Foam::fvMeshTopoChangers::myrefiner::setDumpList
(
    const labelList& vals,
    const word& name
) const
{
    // Resolve member pointer to actual autoPtr
    autoPtr<labelIOList>& lst = const_cast<autoPtr<labelIOList>&>(
        this->*MemberPtr
    );

    lst.reset
    (
        new labelIOList
        (
            IOobject
            (
                name,
                mesh().time().name(),
                mesh().thisDb(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            vals.size()
        )
    );

    forAll(vals, i)
    {
        (*lst)[i] = vals[i];
    }
}
// ************************************************************************* //


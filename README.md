# Refiner topoChanger

Working on adaptive mesh refinement in OpenFOAM.

- [x] Sort candidate cells for refinement based `cellError` *scalarField*.
- [x] Support both single-processor and parallel runs by reconstructing `cellError` field.
- [x] Stop refine cells once user-defined threshold is reached.
- [x] Add synchronization across processors to ensure consistency across boundaries.
- [x] Make available for processing additional information on adaptive mesh refinement
- [x] Introduce a user-defined time-based smoothing to gradually ramp in cell refinement at startup.
- [x] Restore unrefinement based on user-defined threshold `unrefineLevel`. Base field is processed using `cellToPoint` function.


---

### Note: differences between `dynamicRefineFvMesh` and the new `refiner`

- The field value used to mark cells for refinement isn't processed the same way as in the original `dynamicRefineFvMesh` class. No smoothing applied as mentioned in [esi-version](https://github.com/ptava/dynamicFvMesh.git). 

- `cellZone` definition for refinement/unrefinement operations is supported.

---


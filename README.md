# Refiner topoChanger

Working on adaptive mesh refinement in OpenFOAM.

- [x] Sort candidate cells for refinement based on `cellError` *scalarField*.
- [x] Support both single-processor and parallel runs by reconstructing `cellError` field.
- [x] Stop refine cells once user-defined threshold is reached.
- [x] Add synchronization across processors to ensure consistency across boundaries.
- [x] Make available for processing additional information on adaptive mesh refinement
- [x] Introduce a user-defined time-based smoothing to gradually ramp in cell refinement at startup, still supporting sorting candidate cells for refinement.
- [x] Unrefinement based on user-defined threshold `unrefineLevel` and processed field values.
- [x] Synchronize unrefinement selection across processors (is it really needed?).
- [x] User can now change run-time parameters in `dynamicMeshDict`
- [x] New dictionary entry `unrefineInterval` to control how often unrefinement is performed

---

### Why adaptive mesh refinement?
<p align="center">
    <img src="assets/imgs/amr_why.png" width="90%" height="90%">
</p>

### Refiner library visual representation
<p align="center">
    <img src="assets/imgs/amr.png" width="90%" height="90%">
</p>

---
### Examples

#### Q-criterion based refinement
<div align="center">
    <table>
      <tr>
        <td align="center">
            <img src="assets/videos/posQ.gif" alt="Video 1 preview" width="700">
          <br>
          <sub><b>Refine vortices-dominated region</b></sub>
        </td>
      </tr>
      <tr>
        <td align="center">
            <img src="assets/videos/negQ.gif" alt="Video 2 preview" width="700">
          <br>
          <sub><b>Refine in strain-dominated region</b></sub>
        </td>
      </tr>
      <tr>
        <td align="center">
            <img src="assets/videos/absQ.gif" alt="Video 3 preview" width="700">
          <br>
          <sub><b>Refine both vortex/strain-dominated regions</b></sub>
        </td>
      </tr>
    </table>
</div>

#### Von Kàrmàn length scale based refinement

<div align="center">
    <table>
      <tr>
        <td align="center">
            <img src="assets/videos/lvk.gif" alt="Video 1 preview" width="700">
          <br>
          <sub><b>Refine where the high-wave-number filter is active</b></sub>
        </td>
      </tr>
    </table>
</div>

---

> [!NOTE]
> - `cellZone` definition for refinement/unrefinement operations is supported.
> 
> - removed the usage of `extendMarkedCells` function, in my believe it is not necessary anymore: preprocess input field to expand focus region of refinement criteria to avoid abrupt refinements. Now `nBufferLayers` isn't used anymore, could be used in `meshCutter.consistentRefinement` logic to expand the 2:1 refinement buffer.
> 
> - If number of selected cells does not exceed the available budget, we have a check on maximum level of refinement.
> 
> - Selected cells for refinement are filtered by the scale value defined by user. So in case we have available spots for all "filtered" cells, we still have to select them based on the sorted field values.
> 
> - sorting part of the algorithm and candidates selection (in parallel) are not computationally efficient
> 
> - Candidates cells for unrefinement are now selected based on the field values, with additional synchronization across processors.
> 
> - Available `uniformDimensionedScalarField` for additional post-processing via `dumpRefinementInfo` flag (updated each iteration of refinement/unrefinement):
>     * `nCells`: number of cells at the end of refinement/unrefinement process
>     * `nTotToRefine`: maximum number of cells at that can be generated based on user-defined `maxCells` threshold
>     * `nCandidates`: number of candidate cells for refinement before applying any threshold
>     * `nSelected`: number of candidate cells after maximum cells threshold applied and scale filtering (respectively with `maxCells` and `refinementScale` user-defined parameters)
>     * `nToTRefined`: number of cells actually refined in that iteration (after 2:1 consistency and after checking maximum refinement level allowed)
>     * `nToUnrefined`: number of cells unrefined
>     * `lowerLimit`: lower limit threshold applied on `cellError`
>     * `upperLimit`: upper limit threshold applied on `cellError`
>     * `nAtMaxRefinement`: number of cells at maximum refinement level (after refinement))
> 
> - Available `volScalarField` associated with `cellError` for additional post-processing via `dumpRefinementFields` flag.
> 
> - Available `labelIOList` of refined and unrefined cells with debugging flag
> 
> - Need to check the behaviour with `refinementRegions` definition (which changed in later version of OpenFOAM)
> 
> - Experienced instabilities with time-step adaptation via `adjustTimeStep`(-> `maxCo`) leading to simulation crash. Need to change the logic to avoid abrupt changes or fix directly the time-step
---


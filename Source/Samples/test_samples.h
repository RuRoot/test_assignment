//////////////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief  \ru Демонстрационные примеры.
          \en Some functionality samples. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_SAMPLES_H
#define __TEST_SAMPLES_H


//////////////////////////////////////////////////////////////////////////////////////////
//
//  Modeler Samples
//
//////////////////////////////////////////////////////////////////////////////////////////

void SampleAttributes();                // How to use attributes.
void SampleTorusSplineSolid();          // How to create a torus solid based on the toroidal spline-surface.
void SampleSplitBallIntoEightPieces();  // How to split the ball into 8 pieces.
void SampleSplitCylinderManualSplit();  // How to create a split face on the cylinder face.
void SampleSolidsClassification();      // How to use multiple solids classification.
void SampleCuttingEdgeWireframe();      // How to cut an edge of a wireframe.
void SampleReadWriteModelAsPart();      // How to read and write c3d data as part of binary file.
void SampleGridSurface();               // How to create a face on a surface by triangulating the points of a pyramid with a quadrangular base.
void SampleCharacterCurve();            // How to create the curve by analytical functions of coordinates.
void SampleAnimationOfBending();        // How to create the animation of sheet bending.
void SampleLoftedBend();                // How to create a lofted bend.
void SampleCloseCorner();               // How to close a corner between two bends.
void SampleSeveralBends();              // How to fold several bends simultaneously.


//////////////////////////////////////////////////////////////////////////////////////////
//
//  Solver Samples
//
//////////////////////////////////////////////////////////////////////////////////////////

void SampleParametricSketch(); // How to create parametric sketch.
void SampleWriteSketch();      // How to write parametric sketch.
void SampleReadSketch();       // How to read parametric sketch.
void SampleSpinningBlock();    // How to spin a block using constraints.
void SampleDodecahedron();     // How to assembly dodecahedron using the solver.

//////////////////////////////////////////////////////////////////////////////////////////
//
//  Converter Samples
//
//////////////////////////////////////////////////////////////////////////////////////////

void SampleImportExport(); // How to import and export models using exchange format files.
void SampleWritePMI(); // How to manage PMI objects

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tools Samples
//
//////////////////////////////////////////////////////////////////////////////////////////

void SampleCollisionDetection();  // How to apply collision detection utility
void SampleCheckCollisions();
void SampleCollisionDetectionGroups();

//////////////////////////////////////////////////////////////////////////////////////////
//
// Polymesh Samples
//
//////////////////////////////////////////////////////////////////////////////////////////

void SamplePolymeshIcosahedron();  // How to create and sew polymesh
void SamplePolymeshDartboard();  // How to cut polymesh by curves
void SamplePolymeshOffsetPath();  // How to offset path on the polymesh

#endif // __TEST_SAMPLES_H
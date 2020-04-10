//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Modifications copyright (C) 2020 Autodesk
//
#ifndef PXRUSDMAYA_MESH_WRITE_UTILS_H
#define PXRUSDMAYA_MESH_WRITE_UTILS_H

#include <mayaUsd/base/api.h>

#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MObject.h>
#include <maya/MString.h>

#include <pxr/pxr.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/tf/token.h>
#include <pxr/base/vt/array.h>
#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdUtils/sparseValueWriter.h>

PXR_NAMESPACE_OPEN_SCOPE

class UsdGeomMesh;

// Utilities for dealing with writing USD from Maya mesh/subdiv tags.
namespace UsdMayaMeshUtil
{
    /// Helper method for getting Maya mesh normals as a VtVec3fArray.
    MAYAUSD_CORE_PUBLIC
    bool getMeshNormals(const MFnMesh& mesh, VtArray<GfVec3f>* normalsArray, TfToken* interpolation);

    /// Gets the subdivision scheme tagged for the Maya mesh by consulting the
    /// adaptor for \c UsdGeomMesh.subdivisionSurface, and then falling back to
    /// the RenderMan for Maya attribute.
    MAYAUSD_CORE_PUBLIC
    TfToken getSubdivScheme(const MFnMesh &mesh);

    /// Gets the subdivision interpolate boundary tagged for the Maya mesh by
    /// consulting the adaptor for \c UsdGeomMesh.interpolateBoundary, and then
    /// falling back to the RenderMan for Maya attribute.
    MAYAUSD_CORE_PUBLIC
    TfToken getSubdivInterpBoundary(const MFnMesh &mesh);

    /// Gets the subdivision face-varying linear interpolation tagged for the
    /// Maya mesh by consulting the adaptor for
    /// \c UsdGeomMesh.faceVaryingLinearInterpolation, and then falling back to
    /// the OpenSubdiv2-style tagging.
    MAYAUSD_CORE_PUBLIC
    TfToken getSubdivFVLinearInterpolation(const MFnMesh& mesh);

    MAYAUSD_CORE_PUBLIC
    bool isMeshValid(const MDagPath&);

    MAYAUSD_CORE_PUBLIC
    void exportReferenceMesh(UsdGeomMesh&, MObject);

    MAYAUSD_CORE_PUBLIC
    void assignSubDivTagsToUSDPrim(MFnMesh&, UsdGeomMesh&, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    void writeSubdivInterpBound(MFnMesh&, UsdGeomMesh&, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    void writeSubdivFVLinearInterpolation(MFnMesh&, UsdGeomMesh&, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    void writeVertexData(const MFnMesh&, UsdGeomMesh&, const UsdTimeCode&, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    void writeFaceVertexIndicesData(const MFnMesh&, UsdGeomMesh&, const UsdTimeCode&, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    void writeInvisibleFacesData(const MFnMesh&, UsdGeomMesh&, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    void writeNormalsData(const MFnMesh&, UsdGeomMesh&, const UsdTimeCode&, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    bool getMeshUVSetData(const MFnMesh&, const MString&, VtArray<GfVec2f>*, TfToken*, VtArray<int>*);

    MAYAUSD_CORE_PUBLIC
    bool writeUVSetsAsVec2fPrimvars(const MFnMesh&, UsdGeomMesh&, const UsdTimeCode&, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    bool addDisplayPrimvars(UsdGeomGprim&, const UsdTimeCode&, const MFnMesh::MColorRepresentation,
                            const VtArray<GfVec3f>&, const VtArray<float>&, const TfToken&,
                            const VtArray<int>&, const bool, const bool, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    bool createRGBPrimVar(UsdGeomGprim&, const TfToken&, const UsdTimeCode&,
                          const VtArray<GfVec3f>&, const TfToken&, const VtArray<int>&,
                          bool, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    bool createRGBAPrimVar(UsdGeomGprim&, const TfToken&, const UsdTimeCode&,
                           const VtArray<GfVec3f>&, const VtArray<float>&, 
                           const TfToken&,const VtArray<int>&,bool, UsdUtilsSparseValueWriter&);

    MAYAUSD_CORE_PUBLIC
    bool createAlphaPrimVar(UsdGeomGprim&, const TfToken&, const UsdTimeCode&,
                            const VtArray<float>&, const TfToken&,const VtArray<int>&,
                            bool, UsdUtilsSparseValueWriter&);

    /// Collect values from the color set named \p colorSet.
    /// If \p isDisplayColor is true and this color set represents displayColor,
    /// the unauthored/unpainted values in the color set will be filled in using
    /// the shader values in \p shadersRGBData and \p shadersAlphaData if available.
    /// Values are gathered per face vertex, but then the data is compressed to
    /// vertex, uniform, or constant interpolation if possible.
    /// Unauthored/unpainted values will be given the index -1.
    MAYAUSD_CORE_PUBLIC
    bool getMeshColorSetData( MFnMesh&, const MString&, bool, const VtArray<GfVec3f>&, const VtArray<float>&,
                              const VtArray<int>&, VtArray<GfVec3f>*, VtArray<float>*, TfToken*, VtArray<int>*,
                              MFnMesh::MColorRepresentation*, bool*);

} // namespace UsdMayaMeshUtil

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDMAYA_MESH_WRITE_UTILS_H

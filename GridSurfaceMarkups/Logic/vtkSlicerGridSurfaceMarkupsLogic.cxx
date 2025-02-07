/*==============================================================================

  Distributed under the OSI-approved BSD 3-Clause License.

  Copyright (c) Children's Hospital of Philadelphia. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  * Neither the name of Kitware, Inc. nor the names of Contributors
    may be used to endorse or promote products derived from this
    software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  This file was originally developed by Csaba Pinter (Pixel Medical /
  Ebatinca).

==============================================================================*/

#include "vtkSlicerGridSurfaceMarkupsLogic.h"

// GridSurface Markups MRML includes
#include "vtkMRMLMarkupsGridSurfaceNode.h"

// MRML includes
#include <vtkMRMLModelNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSelectionNode.h>

// Markups logic includes
#include <vtkSlicerMarkupsLogic.h>

// Markups MRML includes
#include <vtkMRMLMarkupsDisplayNode.h>

// VTK includes
#include <vtkObjectFactory.h>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerGridSurfaceMarkupsLogic);

//---------------------------------------------------------------------------
vtkSlicerGridSurfaceMarkupsLogic::vtkSlicerGridSurfaceMarkupsLogic()
{

}

//---------------------------------------------------------------------------
vtkSlicerGridSurfaceMarkupsLogic::~vtkSlicerGridSurfaceMarkupsLogic() = default;

//---------------------------------------------------------------------------
void vtkSlicerGridSurfaceMarkupsLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//-----------------------------------------------------------------------------
void vtkSlicerGridSurfaceMarkupsLogic::RegisterNodes()
{
  std::cout << "Register Nodes" << std::endl;

  assert(this->GetMRMLScene() != nullptr);

  vtkMRMLScene *scene = this->GetMRMLScene();

  // Nodes
  scene->RegisterNodeClass(vtkSmartPointer<vtkMRMLMarkupsGridSurfaceNode>::New());
}

//---------------------------------------------------------------------------
void vtkSlicerGridSurfaceMarkupsLogic::ObserveMRMLScene()
{
  if (!this->GetMRMLScene())
  {
    return;
  }

  vtkMRMLApplicationLogic* mrmlAppLogic = this->GetMRMLApplicationLogic();
  if (!mrmlAppLogic)
  {
    vtkErrorMacro("ObserveMRMLScene: invalid MRML Application Logic.");
    return;
  }

  vtkMRMLNode* node = this->GetMRMLScene()->GetNodeByID(this->GetSelectionNodeID().c_str());
  if (!node)
  {
    vtkErrorMacro("Observe MRMLScene: invalid Selection Node");
    return;
  }

  // add known markup types to the selection node
  vtkMRMLSelectionNode* selectionNode =
    vtkMRMLSelectionNode::SafeDownCast(node);
  if (selectionNode)
  {
    // got into batch process mode so that an update on the mouse mode tool
    // bar is triggered when leave it
    this->GetMRMLScene()->StartState(vtkMRMLScene::BatchProcessState);

    auto gridSurfaceNode = vtkSmartPointer<vtkMRMLMarkupsGridSurfaceNode>::New();
    selectionNode->AddNewPlaceNodeClassNameToList(gridSurfaceNode->GetClassName(),
      gridSurfaceNode->GetAddIcon(),
      gridSurfaceNode->GetMarkupType());

    // trigger an update on the mouse mode toolbar
    this->GetMRMLScene()->EndState(vtkMRMLScene::BatchProcessState);
  }

 this->Superclass::ObserveMRMLScene();
}

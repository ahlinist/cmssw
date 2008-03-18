// -*- C++ -*-
//
// Package:     Core
// Class  :     FWDetailViewManager
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Wed Mar  5 09:13:47 EST 2008
// $Id: FWDetailViewManager.cc,v 1.3 2008/03/07 01:05:16 jmuelmen Exp $
//

// system include files
#include <stdio.h>

// user include files
#include "TGButton.h"
#include "TGFrame.h"
#include "TGLEmbeddedViewer.h"
#include "TEveManager.h"
#include "TEveScene.h"
#include "TEveViewer.h"
#include "Fireworks/Core/interface/FWDetailViewManager.h"
#include "Fireworks/Core/interface/FWDetailView.h"
#include "Fireworks/Core/interface/FWModelId.h"
#include "Fireworks/Core/interface/FWEventItem.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FWDetailViewManager::FWDetailViewManager()
{
}

// FWDetailViewManager::FWDetailViewManager(const FWDetailViewManager& rhs)
// {
//    // do actual copying here;
// }

FWDetailViewManager::~FWDetailViewManager()
{
}

//
// assignment operators
//
// const FWDetailViewManager& FWDetailViewManager::operator=(const FWDetailViewManager& rhs)
// {
//   //An exception safe implementation is
//   FWDetailViewManager temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void FWDetailViewManager::registerDetailView (const std::string &item_name, 
					      FWDetailView *view)
{
     m_viewers[item_name] = view;
}

void FWDetailViewManager::close_wm ()
{
     printf("mmmm, flaming death!\n");
     frame = 0;
}

void FWDetailViewManager::close_button ()
{
     printf("mmmm, flaming death!\n");
     frame->CloseWindow();
     frame = 0;
}

void 
FWDetailViewManager::openDetailViewFor(const FWModelId &id)
{
     printf("opening detail view for event item %s (%x), index %d\n", 
	    id.item()->name().c_str(), (unsigned int)id.item(), id.index());

     // make a frame
     frame = new // TGTransientFrame(0, gEve->GetBrowser(), 400, 400);
	  TGMainFrame(0, 400, 420);
     // connect the close-window button to something useful
     frame->Connect("CloseWindow()", "FWDetailViewManager", this, "close_wm()");
     frame->SetCleanup(kDeepCleanup);
     TGLEmbeddedViewer* v = new TGLEmbeddedViewer(frame, 0);
     TEveViewer* nv = new TEveViewer();
     nv->SetGLViewer(v);
     nv->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraPerspXOY);
     // nv->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
     nv->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);
     nv->GetGLViewer()->SetClearColor(kBlack);
     ns = gEve->SpawnNewScene("Detailed view");
     nv->AddScene(ns);
     frame->AddFrame(v->GetFrame(), 
		     new TGLayoutHints(kLHintsTop | kLHintsExpandX
				       | kLHintsExpandY));
     TGTextButton* exit_butt = new TGTextButton(frame, "Eat flaming death");
     exit_butt->Resize(20, 20);
     exit_butt->Connect("Clicked()", "FWDetailViewManager", this, "close_button()");
     frame->AddFrame(exit_butt, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
     frame->Layout();
     frame->SetWindowName("Ooogadooga-WINDOW");
     frame->SetIconName("Ooogadooga-ICON");
     frame->MapSubwindows();
     frame->MapWindow();

     // find the right viewer for this item
     std::map<std::string, FWDetailView *>::iterator viewer = 
	  m_viewers.find(id.item()->name());
     if (viewer == m_viewers.end()) {
	  std::cout << "FWDetailViewManager: don't know what detailed view to "
	       "use for object " << id.item()->name() << std::endl;
	  assert(viewer != m_viewers.end());
     }

     // run the viewer
     TEveElementList *list = 0;
     viewer->second->build(&list, id);
     gEve->AddElement(list, ns);
}

//
// const member functions
//
bool 
FWDetailViewManager::haveDetailViewFor(const FWModelId& iId) const
{
   return m_viewers.end() != m_viewers.find(iId.item()->name());
}

//
// static member functions
//

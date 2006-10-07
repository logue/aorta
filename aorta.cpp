/*

  aorta.cpp: the Aleph One Replacement Texture Utility
  Copyright (C) 2006  Gregory Smith

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 
*/
#include "aorta.h" 
#include "DDSOptionsDialog.h"
#include "imagdds.h"

IMPLEMENT_APP(MainApp)  

extern bool HasS3TC(); // from imagdds

bool MainApp::OnInit() 
{ 
	::wxInitAllImageHandlers();
	wxDDSHandler *ddsHandler = new wxDDSHandler;
	wxImage::AddHandler(ddsHandler);
	MainWin = new MainFrame(_("Aorta"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~ (wxRESIZE_BORDER | wxRESIZE_BOX | wxMAXIMIZE_BOX)); 
	MainWin->Show(TRUE); 
	SetTopWindow(MainWin); 

	if (argc > 1)
		MainWin->LoadNormal(argv[1]);

	if (!HasS3TC())
		wxMessageBox(_T("Can not locate Squish Framework; DXTC support is not available."), _T("Warning"), wxOK);
	
	return TRUE;
} 

#ifdef __WXMAC__
void MainApp::MacOpenFile(const wxString &fileName)
{
	MainWin->LoadNormal(fileName);
}

#endif

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size, long style) 
  : wxFrame((wxFrame *) NULL, -1, title, pos, size, style) 
{ 
	fileMenu = new wxMenu;
	fileMenu->Append(wxID_ABOUT, _T("&About Aorta..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(MENU_LoadNormal, _T("L&oad image..."));
	fileMenu->Append(MENU_SaveAs, _T("&Save as..."));
	fileMenu->Append(wxID_EXIT, _T("&Quit"));

	menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, _T("&File"));
	SetMenuBar(menuBar);
	
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
	notebook = new wxNotebook(this, -1);
	topsizer->Add(notebook, 1, wxEXPAND | wxALL, 10);
	
	basicPage = new BasicPage(notebook, -1, wxDefaultPosition, wxDefaultSize);
	notebook->AddPage(basicPage, _("Basic"), true);
	
	SetAutoLayout(TRUE);
	SetSizer(topsizer);
	topsizer->Fit(this);
	topsizer->SetSizeHints(this);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
	Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(_T("Aorta (the Aleph One Replacement Texture Accessory)\n(C) 2006 Gregory Smith\n\nAorta is licensed under the GPL. See COPYING.txt"), _T("About Aorta"), wxOK);
}

void MainFrame::OnLoadNormal(wxCommandEvent& event)
{
	basicPage->OnLoadNormal(event);
}

void MainFrame::OnSaveAs(wxCommandEvent& event)
{
	basicPage->OnSaveAs(event);
}

void MainFrame::OnLoadMask(wxCommandEvent& event)
{
	basicPage->OnLoadMask(event);
}

void MainFrame::LoadNormal(const wxString& path)
{
	basicPage->LoadNormal(path);
}

BasicPage::BasicPage(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
  : wxNotebookPage(parent, id, pos, size)
{
	wxBoxSizer *pageSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *column[2];
	column[0] = new wxBoxSizer(wxVERTICAL);

	wxImageExt whiteImage(256, 256);
	whiteImage.White();
	
	normalImageStatic = new wxStaticBitmap(this, -1, wxBitmap(whiteImage));
#ifdef wxUSE_DRAG_AND_DROP
	normalImageStatic->SetDropTarget(new DnDNormalImage(this));
#endif	
	column[0]->Add(normalImageStatic, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 10);
	normalImageFilename = new wxStaticText(this, -1, _T(""));
	column[0]->Add(normalImageFilename, 0, wxALIGN_CENTER | wxEXPAND | wxLEFT | wxRIGHT, 10);
	normalImageSize = new wxStaticText(this, -1, _T(""));
	column[0]->Add(normalImageSize, 0, wxALIGN_CENTER | wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
	normalImageButton = new wxButton(this, BUTTON_NormalImage, _T("Load normal..."));
	column[0]->Add(normalImageButton, 0, wxALIGN_CENTER | wxALL, 10);
	
	unpremultiplyAlpha = new wxButton(this, BUTTON_UnpremultiplyAlpha, _T("Un-Premultiply Alpha"));
	column[0]->Add(unpremultiplyAlpha, 0, wxALIGN_CENTER | wxALL, 10);
	
	pageSizer->Add(column[0], 0, wxEXPAND | wxALL, 10);
	
	column[1] = new wxBoxSizer(wxVERTICAL);
	
	maskImageStatic = new wxStaticBitmap(this, -1, wxBitmap(whiteImage));
#ifdef wxUSE_DRAG_AND_DROP
	maskImageStatic->SetDropTarget(new DnDMask(this));
#endif
	column[1]->Add(maskImageStatic, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 10);

	maskStatus = new wxStaticText(this, -1, _T(""));
	column[1]->Add(maskStatus, 0, wxALIGN_CENTER | wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
	column[1]->AddSpacer(maskStatus->GetRect().GetHeight());
	
	wxBoxSizer *loadClearSizer = new wxBoxSizer(wxHORIZONTAL);
	maskImageButton = new wxButton(this, BUTTON_MaskImage, _T("Load mask..."));
	loadClearSizer->Add(maskImageButton, 1, wxEXPAND | wxRIGHT, 10);
	clearMaskButton = new wxButton(this, BUTTON_ClearMask, _T("Clear mask"));
	loadClearSizer->Add(clearMaskButton, 1, wxEXPAND | wxLEFT, 10);
	column[1]->Add(loadClearSizer, 0, wxEXPAND | wxALIGN_CENTER | wxALL, 10);
	
	wxBoxSizer *opacTypeSizer = new wxBoxSizer(wxHORIZONTAL);
	opacTypeTwo = new wxButton(this, BUTTON_OpacTypeTwo, _T("Opac_type=2"));
	opacTypeThree = new wxButton(this, BUTTON_OpacTypeThree, _T("Opac_type=3"));
	opacTypeSizer->Add(opacTypeTwo, 1, wxEXPAND | wxRIGHT, 10);
	opacTypeSizer->Add(opacTypeThree, 1, wxEXPAND | wxLEFT, 10);
	column[1]->Add(opacTypeSizer, 0, wxEXPAND | wxALIGN_CENTER | wxALL, 10);
	SetMaskButtonEnablement(false);
	
	wxBoxSizer *saveSizer = new wxBoxSizer(wxHORIZONTAL);
	saveAsButton = new wxButton(this, BUTTON_SaveAs, _T("Save as..."));
	saveAsButton->Disable();
	saveSizer->AddStretchSpacer(1);
	saveSizer->Add(saveAsButton, 1, wxEXPAND | wxLEFT, 10);
	column[1]->Add(saveSizer, 0, wxEXPAND | wxALIGN_CENTER | wxALL, 10);

	pageSizer->Add(column[1], 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
	
	SetAutoLayout(TRUE);
	SetSizer(pageSizer);
	pageSizer->Fit(this);
	pageSizer->SetSizeHints(this);
	Layout();
}

void BasicPage::OnLoadNormal(wxCommandEvent &)
{
	wxConfig config;
	wxString Directory;
	config.Read("Single/DefaultDirectory/Load", &Directory, "");
	wxFileDialog *openFileDialog = new wxFileDialog( this,
							 _("Choose Image"),
							 Directory,
							 _(""),
							 _("Image Files ") + wxImage::GetImageExtWildcard() + _T("|All Files|*.*"),
							 wxOPEN | wxCHANGE_DIR,
							 wxDefaultPosition);
	if (openFileDialog->ShowModal() == wxID_OK)
	{
		LoadNormal(openFileDialog->GetPath());
	}
}


void BasicPage::OnLoadMask(wxCommandEvent &)
{
	
	wxConfig config;
	wxString Directory;
	config.Read("Single/DefaultDirectory/Load", &Directory, "");
	wxFileDialog *openFileDialog = new wxFileDialog(this,
							_T("Choose Mask"),
							_T(""),
							_T(""),
							_T("Image Files ") + wxImage::GetImageExtWildcard(),
							wxOPEN | wxCHANGE_DIR,
							wxDefaultPosition);
	if (openFileDialog->ShowModal() == wxID_OK)
	{
		LoadMask(openFileDialog->GetPath());
	}
}

void BasicPage::OnClearMask(wxCommandEvent &)
{
	maskImage.Destroy();
	UpdateMaskDisplay();
}

void BasicPage::OnOpacTypeTwo(wxCommandEvent &)
{
	maskImage = normalImage;
	maskImage.MakeOpacTypeTwo();
	UpdateMaskDisplay();
}

void BasicPage::OnOpacTypeThree(wxCommandEvent &)
{
	maskImage = normalImage;
	maskImage.MakeOpacTypeThree();
	UpdateMaskDisplay();
}

void BasicPage::OnUnpremultiplyAlpha(wxCommandEvent &)
{
	normalImage.UnpremultiplyAlpha();
	UpdateNormalDisplay();
}

void BasicPage::OnSaveAs(wxCommandEvent &)
{
	if (!normalImage.Ok()) return;

	wxConfig config;
	wxString Directory;
	config.Read("Single/DefaultDirectory/Save", &Directory, "");
	wxFileDialog *saveFileDialog = new wxFileDialog(this,
							_T("Save As"),
							Directory,
							(normalImageFilename->GetLabel().BeforeLast('.') + ".dds"),
							_T("DDS files (*.dds)|*.dds|PNG files (*.png)|*.png"),
							wxSAVE | wxOVERWRITE_PROMPT | wxCHANGE_DIR,
							wxDefaultPosition);
	if (saveFileDialog->ShowModal() != wxID_OK) return;

	Directory = saveFileDialog->GetFilename().BeforeLast('/');
	config.Write("Single/DefaultDirectory/Save", Directory);
    
	wxImageExt saveImage = normalImage;
	if (maskImage.Ok())
	{
		maskImage.ToAlpha(saveImage);
//			saveImage.PrepareForMipmaps();
	}
	else
	{
		if (saveImage.HasAlpha())
		{
			exit(0);
		}
	}

	if (saveFileDialog->GetFilename().AfterLast('.').MakeLower() == _T("png")) {
		saveImage.SaveFile(saveFileDialog->GetPath(), wxBITMAP_TYPE_PNG);
	} else {
		// query for a preset
		DDSOptionsDialog ddsOptions;
		if (ddsOptions.ShowModal() != wxID_OK) return;
	
		if (ddsOptions.generateMipmaps->GetValue()) {
			saveImage.SetOption(wxIMAGE_OPTION_DDS_USE_MIPMAPS, 1);
			
			if (ddsOptions.premultiplyAlpha->GetValue()) {
				saveImage.SetOption(wxIMAGE_OPTION_DDS_PREMULTIPLY_ALPHA, 1);
			} else {
				saveImage.SetOption(wxIMAGE_OPTION_DDS_PREMULTIPLY_ALPHA, 0);
			}

			if (ddsOptions.colorFillBackground->GetValue()) {
				if (ddsOptions.reconstructColors->GetValue()) {
					saveImage.ReconstructColors(ddsOptions.backgroundColor);
				}

				saveImage.PrepareForMipmaps();
			}
			
		} else {
			saveImage.SetOption(wxIMAGE_OPTION_DDS_USE_MIPMAPS, 0);
		}

		if (HasS3TC() && ddsOptions.useDXTC->GetValue()) {
			saveImage.SetOption(wxIMAGE_OPTION_DDS_COMPRESS, 1);
		} else {
			saveImage.SetOption(wxIMAGE_OPTION_DDS_COMPRESS, 0);
		}

		saveImage.SaveFile(saveFileDialog->GetPath(), wxDDSHandler::wxBITMAP_TYPE_DDS);
	}
}

void BasicPage::LoadNormal(const wxString& path)
{
	wxConfig config;
	config.Write("Single/DefaultDirectory/Load", path.BeforeLast('/'));

	normalImage.LoadFile(path);
	if (normalImage.Ok())
	{
		if (normalImage.HasMask()) normalImage.MaskToAlpha();
	
		if (normalImage.HasAlpha())
		{
			// convert the mask to grayscale for display
			maskImage.FromAlpha(normalImage);
			normalImage.RemoveAlpha();
		}
		else
		{
			maskImage.Destroy();
		}
	
		normalImageFilename->SetLabel(path.AfterLast('/'));
		wxString dimensionsString;
		dimensionsString.Printf("%ix%i", normalImage.GetWidth(), normalImage.GetHeight());
		normalImageSize->SetLabel(dimensionsString);
		UpdateNormalDisplay();
		UpdateMaskDisplay();
	} 
	else
	{
		normalImage.Destroy();
		maskImage.Destroy();
		UpdateNormalDisplay();
		UpdateMaskDisplay();
	}
}

void BasicPage::LoadMask(const wxString& path)
{
	wxConfig config;
	config.Write("Single/DefaultDirectory/Load", path.BeforeLast('/'));

	maskImage.LoadFile(path);
	if (maskImage.Ok())
	{
		if (maskImage.GetWidth() == normalImage.GetWidth() && maskImage.GetHeight() == normalImage.GetHeight())
		{
			maskImage.MakeOpacTypeTwo();
		}
		else
		{
			wxMessageBox(_T("The mask must be the same width and height as the image."), _T("Invalid mask"), wxOK);
			maskImage.Destroy();
		}
	}
	else
	{
		maskImage.Destroy();
	}
	UpdateMaskDisplay();
}

void BasicPage::SetMaskButtonEnablement(bool enabled)
{
	if (enabled)
	{
		maskImageButton->Enable();
		clearMaskButton->Enable();
		opacTypeTwo->Enable();
		opacTypeThree->Enable();
	}
	else
	{
		maskImageButton->Disable();
		clearMaskButton->Disable();
		opacTypeTwo->Disable();
		opacTypeThree->Disable();
	}
}

void BasicPage::UpdateNormalDisplay()
{
	
	int width, height;
	normalImageStatic->GetSize(&width, &height);
	
	if (normalImage.Ok())
	{
		wxImageExt normalImageDisplay = normalImage;
		normalImageDisplay.PropRescale(width, height);
		normalImageStatic->SetBitmap(wxBitmap(normalImageDisplay));
		
		saveAsButton->Enable();
		SetMaskButtonEnablement(true);
	}
	else
	{
		wxImageExt normalImageDisplay(width, height);
		normalImageDisplay.White();
		normalImageStatic->SetBitmap(wxBitmap(normalImageDisplay));
		
		saveAsButton->Disable();
		SetMaskButtonEnablement(false);
	}
}

void BasicPage::UpdateMaskDisplay()
{
	int width, height;
	maskImageStatic->GetSize(&width, &height);
	
	wxImageExt maskImageDisplay;
	if (maskImage.Ok())
	{
		maskImageDisplay = maskImage;
		maskStatus->SetLabel(_T(""));
	} 
	else if (normalImage.Ok())
	{
		maskImageDisplay.Create(normalImage.GetWidth(), normalImage.GetHeight());
		maskImageDisplay.White();
		maskStatus->SetLabel(_T("No mask"));
	}
	else
	{
		maskImageDisplay.Create(width, height);
		maskImageDisplay.White();
		maskStatus->SetLabel(_T(""));
	}
	maskImageDisplay.PropRescale(width, height);
	maskImageStatic->SetBitmap(wxBitmap(maskImageDisplay));
	
}

bool DnDNormalImage::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
	if (filenames[0])
		m_page->LoadNormal(filenames[0]);
}

bool DnDMask::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
	if (filenames[0])
		m_page->LoadMask(filenames[0]);
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
EVT_MENU(MENU_LoadNormal, MainFrame::OnLoadNormal)
EVT_MENU(MENU_SaveAs, MainFrame::OnSaveAs)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BasicPage, wxNotebookPage)
EVT_BUTTON(BUTTON_NormalImage, BasicPage::OnLoadNormal)
EVT_BUTTON(BUTTON_MaskImage, BasicPage::OnLoadMask)
EVT_BUTTON(BUTTON_ClearMask, BasicPage::OnClearMask)
EVT_BUTTON(BUTTON_OpacTypeTwo, BasicPage::OnOpacTypeTwo)
EVT_BUTTON(BUTTON_OpacTypeThree, BasicPage::OnOpacTypeThree)
EVT_BUTTON(BUTTON_SaveAs, BasicPage::OnSaveAs)
EVT_BUTTON(BUTTON_UnpremultiplyAlpha, BasicPage::OnUnpremultiplyAlpha)
END_EVENT_TABLE()
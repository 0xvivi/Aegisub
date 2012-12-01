// Copyright (c) 2007, Rodrigo Braz Monteiro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Aegisub Group nor the names of its contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Aegisub Project http://www.aegisub.org/

/// @file subs_preview.h
/// @see subs_preview.cpp
/// @ingroup custom_control
///

#include <wx/window.h>
#include <wx/bitmap.h>

#include <libaegisub/scoped_ptr.h>

class AssFile;
class AssStyle;
class SubtitlesProvider;
class VideoProvider;

/// DOCME
/// @class SubtitlesPreview
/// @brief Preview window to show a short string with a given ass style
class SubtitlesPreview : public wxWindow {
	/// The subtitle provider used to render the string
	agi::scoped_ptr<SubtitlesProvider> provider;
	/// Bitmap to render into
	agi::scoped_ptr<wxBitmap> bmp;
	/// The currently display style
	AssStyle* style;
	/// Video provider to render into
	agi::scoped_ptr<VideoProvider> vid;
	/// Current background color
	agi::Color backColour;
	/// Subtitle file containing the style and displayed line
	agi::scoped_ptr<AssFile> subFile;
	/// Line used to render the specified text
	AssDialogue* line;

	/// Regenerate the bitmap
	void UpdateBitmap();
	/// Resize event handler
	void OnSize(wxSizeEvent &event);
	/// Paint event handler
	void OnPaint(wxPaintEvent &);

public:
	/// Set the style to use
	void SetStyle(AssStyle const& style);
	/// Set the text to display
	void SetText(wxString text);
	/// Set the background color
	void SetColour(agi::Color col);

	SubtitlesPreview(wxWindow *parent, wxSize size, int style, agi::Color colour);
	~SubtitlesPreview();
};

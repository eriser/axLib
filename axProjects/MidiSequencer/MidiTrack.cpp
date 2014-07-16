#include "MidiTrack.h"

#include <cstdlib> 

string GetCurrentAppDirectory()
{
  char buf[1024];
  readlink("/proc/self/exe", buf, sizeof(buf)-1);
  string path(buf);
  path = path.substr(0, path.find_last_of("/"));
  path.push_back('/');

  return path;
}

void ExecApplication(const string& app_name)
{
  pid_t p_id = fork();

  // Child.
  if (p_id == 0)                
  {
    execl(app_name.c_str(), 0);
    exit(0);
  }

  // Failed to fork.
  else if (p_id < 0)            
  {
      cerr << "Failed to open " << app_name << endl;
      exit(1);
      // Throw exception
  }
}

MultipleSlider::MultipleSlider(axApp* app, axWindow* parent, 
                               const axRect& rect, 
                               const axColor& bgColor, 
                               const int& barIndex,
                               axEvtFunction(MultipleSliderMsg)& slider_fct):
      axPanel(app, parent, rect),
      // Members.
      _barIndex(barIndex),
      _slider_fct(slider_fct)

{
    _bgColor = bgColor;

    axSliderInfo sld_info("",//"sliderPlain.png",   
                          axSize(8, 8),
                          axColor(0.9, 0.9, 0.9), // Bg
                          axColor(1.0, 1.0, 1.0), // bg hover.
                          axColor(0.9, 0.9, 0.9), // Bg clicking.
                          axColor(0.3, 0.3, 0.3), // Slider normal
                          axColor(0.3, 0.3, 0.3), // Slider hover
                          axColor(0.8, 0.0, 0.0), // Slider clicking.
                          axColor(0.0, 0.0, 0.0), // Slider contour.
                          axColor(0.0, 0.0, 0.0), // COntour
                          axColor(0.9, 0.9, 0.9), // Back Slider
                          axColor(0.9, 0.9, 0.9), // Back Slider contour.
                          4);

    _nSlider = 3;
    int size_x = rect.size.x / double(_nSlider);


    axFlag slider_flags = axSLIDER_FLAG_VERTICAL | 
              axSLIDER_FLAG_CLICK_ANYWHERE | 
                axSLIDER_FLAG_RIGHT_ALIGN;


    axEvtFunction(axSliderMsg) sld_fct(GetOnSlider1Move());
    // function<void(const axSliderMsg&)> sld_fct(GetOnSliderMove());
    sliders[0] = new axSlider(app, this, 
                  axRect(0, 0, size_x, rect.size.y), 
                  axSliderEvents(sld_fct), sld_info, slider_flags);

    sld_fct = GetOnSlider2Move();
    sliders[1] = new axSlider(app, this, 
                  axRect(size_x, 0, size_x, rect.size.y), 
                  axSliderEvents(sld_fct), sld_info, slider_flags);

    sld_fct = GetOnSlider3Move();
    sliders[2] = new axSlider(app, this, 
                axRect(size_x * 2, 0, size_x, rect.size.y), 
                axSliderEvents(sld_fct), sld_info, slider_flags);

    sliders[0]->SetBackgroundAlpha(0.0);
    sliders[1]->SetBackgroundAlpha(0.0);
    sliders[2]->SetBackgroundAlpha(0.0);

    // sliders[2]->Hide();

    SetNumberOfSlider(2);

}


void MultipleSlider::OnSlider1Move(const axSliderMsg& msg)
{
    if (_slider_fct)
    {
        MultipleSliderMsg m;
        m.index = 0;
        m.value = 1.0 - msg.GetValue();
        m.bar_index = _barIndex;

      _slider_fct(m);
    }
}

void MultipleSlider::OnSlider2Move(const axSliderMsg& msg)
{
    if (_slider_fct)
    {
        MultipleSliderMsg m;
        m.index = 1;
        m.value = 1.0 - msg.GetValue();
        m.bar_index = _barIndex;

      _slider_fct(m);
    }
}

void MultipleSlider::OnSlider3Move(const axSliderMsg& msg)
{
    if (_slider_fct)
    {
        MultipleSliderMsg m;
        m.index = 2;
        m.value = 1.0 - msg.GetValue();
        m.bar_index = _barIndex;

      _slider_fct(m);
    }
}

void MultipleSlider::SetNumberOfSlider(const int& nb)
{
  _nSlider = nb;

  if(_nSlider == 1)
  {
    axSize rSize(GetRect().size);
    int size_x = rSize.x / double(_nSlider);
    sliders[0]->ResizeSlider(axSize(size_x, rSize.y));

    sliders[1]->Hide();
    sliders[2]->Hide();
  }
  else if(_nSlider == 2)
  {
    axSize rSize(GetRect().size);
    int size_x = rSize.x / double(_nSlider);
    sliders[0]->ResizeSlider(axSize(size_x, rSize.y));
    sliders[1]->ResizeSlider(axSize(size_x, rSize.y));
    sliders[1]->SetPosition(axPoint(size_x, 0));
    sliders[1]->Show();
    sliders[2]->Hide();
  }
  else if(_nSlider == 3)
  {
    axSize rSize(GetRect().size);
    int size_x = rSize.x / double(_nSlider);
    sliders[0]->ResizeSlider(axSize(size_x, rSize.y));
    sliders[1]->ResizeSlider(axSize(size_x, rSize.y));
    sliders[1]->SetPosition(axPoint(size_x, 0));
    
    sliders[1]->Show();
    sliders[2]->Show();
  }
  Update();
}

void MultipleSlider::OnPaint()
{
  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  // Draw background.
  gc->SetColor(_bgColor, 1.0);
  gc->DrawRectangle(rect0);

  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
  gc->DrawRectangleContour(rect0);
}


//------------------------------------------------------------------------------
// MidiTrackSequence.
//------------------------------------------------------------------------------
MidiTrackSequence::MidiTrackSequence(axApp* app, axWindow* parent, 
                  const axRect& rect, Audio* audio, int num):
                  axPanel(app, parent, rect),
                  _track_number(num)
{
  _audio = static_cast<AudioMidiSeq*>(audio);
  _nbars = 16;
  _highLightIndex = -1;
  _notes.resize(_nbars);
  _hightColor = axColor(0.7, 0.7, 0.7);
  _nSubTrack = 2;
  _selectedSeparationNumber = 0;

  _bgImg = new axImage("btn.png");

}

void MidiTrackSequence::SetColorSelection(const ColorChoice& color)
{
  _choice = color;
}

void MidiTrackSequence::OnMouseMotion(const axPoint& mousePos)
{
  axPoint pos = mousePos - GetAbsoluteRect().position;

  int index = pos.x / double(GetRect().size.x) * _nbars;
  int selected = (pos.y / double(GetRect().size.y)) * _nSubTrack;

  if(index != _highLightIndex || selected != _selectedSeparationNumber)
  {
    _highLightIndex = index;
    _selectedSeparationNumber = selected;
    Update();
  }
}

void MidiTrackSequence::OnMouseLeftDown(const axPoint& mousePos)
{
  if(_highLightIndex != -1)
  {
    _notes[_highLightIndex].active = true;
    // audio->SetBeat(_track_number, _highLightIndex, true);

    if(_notes[_highLightIndex].actives[_selectedSeparationNumber] == true)
    {
      _notes[_highLightIndex].actives[_selectedSeparationNumber] = false;

      // AUDIO CALL.
      _audio->SetMidiNote(_track_number, _selectedSeparationNumber, _highLightIndex, false);      
          
    }
    else
    {
      _notes[_highLightIndex].actives[_selectedSeparationNumber] = true;

      if(_choice == CHOICE_RED)
      {
        _notes[_highLightIndex].colors[_selectedSeparationNumber] = axColor(0.8, 0.0, 0.0);
      }
      else if(_choice == CHOICE_GREEN)
      {
        _notes[_highLightIndex].colors[_selectedSeparationNumber] = axColor(0.0, 0.8, 0.0);
      } 
      else if(_choice == CHOICE_BLUE)
      {
        _notes[_highLightIndex].colors[_selectedSeparationNumber] = axColor(0.0, 0.0, 0.8);
      } 

      // AUDIO CALL.
      _audio->SetMidiNote(_track_number, _selectedSeparationNumber, _highLightIndex, true);

      if(_choice == CHOICE_RED)
      {
        _audio->SetProbability(_track_number, _selectedSeparationNumber, _highLightIndex, 1.0);
      }
      else if(_choice == CHOICE_GREEN)
      {
        _audio->SetProbability(_track_number, _selectedSeparationNumber, _highLightIndex, 0.5);
      }
      else if(_choice == CHOICE_BLUE)
      {
        _audio->SetProbability(_track_number, _selectedSeparationNumber, _highLightIndex, 0.2);
      }
    }

    
    
    Update();
  }
}

void MidiTrackSequence::OnMouseLeave()
{
  if(_highLightIndex != -1)
  {
    _highLightIndex = -1;
    Update();
  }
}

void MidiTrackSequence::DrawMidiSequence(axGC* gc, const axRect& rect0)
{
    axColor colors[3] = {axColor(0.8, 0.0, 0.0), 
                         axColor(0.0, 0.8, 0.0), 
                         axColor(0.0, 0.0, 0.8)};

    for(int i = 0; i < _nbars; i++)
    {
        if(_notes[i].active == true)
        {
            double ratio = rect0.size.x / double(_nbars);
            int left_x = i * ratio;;
            int right_x = (i+1.0) * ratio;


            for(int k = 0; k < _nSubTrack; k++)
            {
                if(_notes[i].actives[k] == true)
                {
                    int pos_y = k * rect0.size.y / double(_nSubTrack);
                    int size_y = rect0.size.y / double(_nSubTrack);

                    // Draw background color.
                    gc->SetColor(_notes[i].colors[k]);
                    gc->DrawRectangle(axRect(left_x, pos_y, right_x - left_x, size_y));

                    // Draw background image.
                    gc->DrawImageResize(_bgImg, 
                                        axPoint(left_x, pos_y), 
                                        axSize(right_x - left_x, size_y), 1.0);
                }
            }
        }
    }
}

void MidiTrackSequence::OnPaint()
{
  string path = GetCurrentAppDirectory();

  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  // Draw background.
  gc->SetColor(axColor("#888888"), 1.0);
  gc->DrawRectangle(rect0);

  
  // Draw hightlighted note.
  if(_highLightIndex != -1)
  {
    int height_y = rect0.size.y / double(_nSubTrack);

    int left_x = (_highLightIndex / double(_nbars)) * rect0.size.x;
    int right_x = ((_highLightIndex+1.0) / double(_nbars)) * rect0.size.x;
    int y_top = height_y * _selectedSeparationNumber;
    int y_bot = height_y * (1.0 + _selectedSeparationNumber);


    if(_notes[_highLightIndex].active == true)
    {
      gc->SetColor(1.0, 0.0, 0.0);
    }
    else
    {
      gc->SetColor(_hightColor);
    }

    gc->DrawRectangle(axRect(left_x, y_top, right_x - left_x, y_bot - y_top));
  }

  // Draw separation.
  int height_y = rect0.size.y / double(_nSubTrack);
  int y_pos = height_y;


  // Draw contour.
  int delta = double(_nbars) * rect0.size.x;
  for(int i = 0; i < _nbars; i++)
  {
    // gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    int x_pos = (i / double(_nbars)) * rect0.size.x;
    // gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect0.size.y));

    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    int x_pos2 = ((i+1) / double(_nbars)) * rect0.size.x;

    if(i % 2)
    {
      // gc->SetColor(axColor(0.8, 0.8, 0.8), 1.0);
        gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
    }
    else
    {
      // gc->SetColor(axColor(0.75, 0.75, 0.75), 1.0);
        gc->SetColor(axColor(0.45, 0.45, 0.45), 1.0);
    }
    
    gc->DrawRectangle(axRect(x_pos, 0, x_pos2 - x_pos, rect0.size.y));
  }

  DrawMidiSequence(gc, rect0);

  // Draw contour.
  for(int i = 0; i < _nbars; i++)
  {
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    int x_pos = (i / double(_nbars)) * rect0.size.x;
    gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect0.size.y));
  }

  gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
  for(int i = 0; i < _nSubTrack; i++)
  {
    gc->DrawLine(axPoint(0.0, y_pos), axPoint(rect0.size.x, y_pos));
    y_pos += height_y;
  }

  
  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
  gc->DrawRectangleContour(rect0);
}

MidiTrackName::MidiTrackName(axApp* app, axWindow* parent, 
               const axRect& rect, 
               const string& name):
      axPanel(app, parent, rect),
      _trackName(name)
{

}


void MidiTrackName::OnPaint()
{
  string path = GetCurrentAppDirectory();

  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
  gc->DrawRectangle(rect0);

  // cout << "TEST" << endl;

  // gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
  gc->SetColor(axColor(0.8, 0.8, 0.8), 1.0);
  // gc->SetFontType(path+string("FreeSans.ttf"));
  gc->SetFontSize(13);
  // gc->DrawString(_trackName, axPoint(0, 0));
  gc->DrawStringAlignedCenter(_trackName, rect0);
  gc->DrawRectangleContour(rect0);
}




//-----------------------------------------------------------------------------
// MidiTrack.
//-----------------------------------------------------------------------------
MidiTrack::MidiTrack(axApp* app, axWindow* parent, const axRect& rect, 
           const string& trackName, Audio* audio, int track_number):
      axPanel(app, parent, rect),
      _track_number(track_number),
      _nSubTrack(2)
      
{

  _audio = static_cast<AudioMidiSeq*>(audio);

  axButtonInfo btn_info(axColor(0.2, 0.2, 0.2),
                        axColor(0.3, 0.3, 0.3),
                        axColor(0.2, 0.2, 0.2),
                        axColor(0.2, 0.2, 0.2),
                        axColor(0.0, 0.0, 0.0),
                        axColor(0.8, 0.8, 0.8));

  function<void (axButtonMsg)> btnFct(GetOnAddSeparation());

  int x = 10, y = 10, xDelta = 25;

  int height = 30.0 / double(_nSubTrack);

  _addBtn = new axButton(app, this, 
               axRect(rect.size.x - 14, 0, 14, height), 
               axButtonEvents(btnFct), 
               btn_info, "", "+");

  btnFct = GetOnRemoveSeparation();
  _removeBtn = new axButton(app, this, 
               axRect(rect.size.x - 14, height, 14, height), 
               axButtonEvents(btnFct), 
               btn_info, "", "-");


  // _trackName = new MidiTrackName(app, this, 
  //                 axRect(0, 0, 60, 30), 
  //                 trackName);

  btnFct = GetOnMinimize();
  axButton* name = new axButton(app, this, 
               axRect(0, 0, 60, 30), 
               axButtonEvents(btnFct), 
               btn_info, "", trackName);

  _trackSeq = new MidiTrackSequence(app, this, 
                    axRect(60, 0, 
                         rect.size.x - 60 - 14, 
                         30), 
                        audio, _track_number);

  axEvtFunction(MultipleSliderMsg) fct(GetOnVelocity());
  _velocity = new MidiVelocity(app, this, axRect(0, 30, 480, 60), fct);


}

void MidiTrack::OnVelocity(const MultipleSliderMsg& msg)
{
    cout << msg.bar_index << " " << msg.index << " " << msg.value << endl;
    _audio->SetVelocity(_track_number, 
                        msg.index, 
                        msg.bar_index, 
                        msg.value);
}

void MidiTrack::OnAddSeparation(const axButtonMsg& msg)
{
  _trackSeq->SetNumberOfSubTrack(++_nSubTrack);
  _velocity->SetNumberOfSlider(_nSubTrack);
  _audio->SetTrackNumberOfSection(_track_number, _nSubTrack);
}

void MidiTrack::OnMinimize(const axButtonMsg& msg)
{
  if(_velocity->IsShown())
  {
    _velocity->Hide();
    SetSize(GetRect().size - axSize(0, 60));

    if (_evt.minimize)
    {
      _evt.minimize(0);
    }

  }
  else
  {
    _velocity->Show();
    SetSize(GetRect().size + axSize(0, 60));

    if (_evt.minimize)
    {
      _evt.minimize(0);
    }
  }

}

void MidiTrack::OnRemoveSeparation(const axButtonMsg& msg)
{
  _nSubTrack--;
  if(_nSubTrack <= 0)
  {
    _nSubTrack = 1;
  }
  _trackSeq->SetNumberOfSubTrack(_nSubTrack);
  _velocity->SetNumberOfSlider(_nSubTrack);
  _audio->SetTrackNumberOfSection(_track_number, _nSubTrack);
}

//------------------------------------------------------------------------

void MidiTrack::SetColorSelection(const ColorChoice& choice)
{
  _trackSeq->SetColorSelection(choice);
}

void MidiTrack::OnPaint()
{
  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  gc->SetColor(axColor("#888800"), 1.0);
  gc->DrawRectangle(rect0);

  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
  gc->DrawRectangleContour(rect0);
}


//-----------------------------------------------------------------------------
// LineSelection.
//-----------------------------------------------------------------------------
LineSelection::LineSelection(axApp* app, axWindow* parent, const axRect& rect):
      axPanel(app, parent, rect)
{
    
}


void LineSelection::OnPaint()
{
  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
  gc->DrawRectangle(rect0);

   gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
   gc->SetFontSize(10);
   // gc->DrawLine(axPoint(1, rect0.size.y), axPoint(1, rect0.size.y - 10));

  for(int i = 0; i < 16; i++)
  {
    int x_pos = ((i) / double(16)) * (rect0.size.x - 14.0);

    // gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
    if(i % 4 == 0)
    {
        gc->DrawLine(axPoint(x_pos, rect0.size.y), axPoint(x_pos, rect0.size.y - 8));
    }
    else
    {
       gc->DrawLine(axPoint(x_pos, rect0.size.y), axPoint(x_pos, rect0.size.y - 6)); 
    }

    int x_pos2 = ((i+1) / double(16)) * (rect0.size.x - 14.0);
    
  // gc->DrawString(_trackName, axPoint(0, 0));
    gc->DrawStringAlignedCenter(to_string(i+1), axRect(x_pos, 5, x_pos2 - x_pos, rect0.size.y - 5));
    
  }
 

  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
  gc->DrawRectangleContour(rect0);
}



//-----------------------------------------------------------------------------
// MidiVelocity.
//-----------------------------------------------------------------------------
MidiVelocity::MidiVelocity(axApp* app,  
                           axWindow* parent, 
                           const axRect& rect, 
                           axEvtFunction(MultipleSliderMsg) fct):
      axPanel(app, parent, rect),
      _velocity_fct(fct)
{
    for(int i = 0; i < 16; i++)
    {
        double ratio = (rect.size.x - 60 - 14) / double(16);
        int left_x = i * ratio;
        int right_x = (i+1.0) * ratio;

        axRect r(60 + left_x, 0, right_x - left_x, rect.size.y);

        axColor color(0.75, 0.75, 0.75);
        if(i % 2)
        {
          color = axColor(0.8, 0.8, 0.8);
        }

        axEvtFunction(MultipleSliderMsg) sldfct(GetOnChangeVelocity());
        _sliders.push_back(new MultipleSlider(app, this, r, color, i, sldfct));
    }
}

void MidiVelocity::OnChangeVelocity(const MultipleSliderMsg& msg)
{
    if(_velocity_fct)
    {
        _velocity_fct(msg);
    }
    
    // cout << msg.bar_index << " " << msg.index << " " << msg.value << endl;
}


void MidiVelocity::OnPaint()
{
  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
  gc->DrawRectangle(rect0);

  gc->SetColor(axColor(0.7, 0.7, 0.7), 1.0);

  for(int i = 0; i < 20; i++)
  {
    int y_pos = (i / double(20)) * (rect0.size.y);
    gc->DrawLine(axPoint(60, y_pos), axPoint(rect.size.x-14, y_pos));
  }

  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);

  for(int i = 0; i < 16+1; i++)
  {
    int x_pos = (i / double(16)) * (rect0.size.x - 60 - 14) + 60;
    gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect0.size.y));
  }

  gc->DrawRectangleContour(rect0);
}
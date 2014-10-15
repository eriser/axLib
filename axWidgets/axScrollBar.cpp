#include "axScrollBar.h"

axScrollBar::axScrollBar(
	axApp* app,
	axWindow* parent,
	const axRect& rect,
	const axScrollBarEvents& events,
	const axScrollBarInfo& info,
	axFlag flags) :
	// Heritage.
	axPanel(app, parent, rect),
	// Members.
	_events(events),
	_info(info),
	_flags(flags)
{

	axButtonInfo btn_info(axColor(0.8, 0.0, 0.0),
						  axColor(0.9, 0.0, 0.0),
						  axColor(0.8, 0.0, 0.0),
						  axColor(0.8, 0.0, 0.0),
						  axColor(0.0, 0.0, 0.0),
						  axColor(0.0, 0.0, 0.0));

	function<void (axButtonMsg)> btnFct();

	_btn[0] = new axButton(app, this, 
						   axRect(0, 0, 14, 14), 
						   axButtonEvents(), 
						   btn_info, 
						   "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/scrollBar_up.png");

	_btn[1] = new axButton(app, this, 
						   axRect(0, rect.size.y - 14, 14, 14), 
						   axButtonEvents(), 
						   btn_info, 
						   "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/scrollBar_down.png");

	m_imgHeight = 14;
	m_sliderMaxHeight = 500 - 2 * m_imgHeight - 1;
	m_yClickDelta = 0;

	// A CHANGER
	m_inputFrameSize = 100;
	m_inputPanelSize = 800;
	m_inputTopDecay = 0;
	m_value = 0;
	m_sliderValue = 0;

	recalculateInputValue();

	// _bgImg = new axImage("gg.png");
}


void axScrollBar::OnMouseLeftDown(const axPoint& position)
{
	// cout << "Left down" << endl;
	axPoint pos = position - GetAbsoluteRect().position;

	if(pos.y > m_sliderPosition && 
	   pos.y < m_sliderPosition + m_sliderHeight)
	{
		// cout << "Mouse in on slider" << endl;
		m_yClickDelta = pos.y - m_sliderPosition;
		GrabMouse();
	}
}

void axScrollBar::OnMouseLeftUp(const axPoint& pos)
{
	if(IsGrabbed())
	{
		UnGrabMouse();
	}
}

void axScrollBar::OnMouseLeftDragging(const axPoint& position)
{
	// cout << "Left draggin" << endl;
	axPoint pos = position - GetAbsoluteRect().position;


	// if(HasCapture() && event.LeftIsDown() && m_sliderHeight < m_sliderMaxHeight)
	if(m_sliderHeight < m_sliderMaxHeight)
	{
		// m_sliderPosition move with mouse position.
		m_sliderPosition = pos.y - m_yClickDelta;

		// If m_sliderPosition reach bottom, clip m_sliderPosition.
		if(m_sliderPosition + m_sliderHeight > GetRect().size.y - 14 - 1)
		{
			m_sliderPosition = GetRect().size.y - 14 - 1 - m_sliderHeight;
		}

		// Clip top m_sliderPosition .
		if(m_sliderPosition < m_imgHeight)
			m_sliderPosition = m_imgHeight;

		// Slider position ratio.
		m_sliderValue = (m_sliderPosition - m_imgHeight) / (m_sliderMaxHeight);
	
		double d = (m_sliderPosition - m_imgHeight) / (m_sliderMaxHeight - m_sliderHeight);
		m_value = ceil(d * (m_inputPanelSize - m_inputFrameSize));

		if(_events.value_change)
		{
			axScrollBarMsg msg(this, to_string(getSliderValue()));
			_events.value_change(msg);
		}
	
		Update();

		// if(m_id != wxID_ANY)
		// {
		// 	wxCommandEvent btnEvent(wxEVT_COMMAND_BUTTON_CLICKED, m_id);
		// 	wxPostEvent(this, btnEvent);
		// }
	}
	else m_value = 0;
}

void axScrollBar::OnMouseEnter()
{
}

void axScrollBar::OnMouseLeave()
{
}

void axScrollBar::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
	gc->DrawRectangle(rect0);

	// gc->DrawImageResize(_bgImg, rect0.position, rect0.size);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(axRect(axPoint(1, 1), rect0.size - axSize(1, 1) ));

	gc->SetColor(axColor(0.5, 0.5, 0.5), 1.0);
	axRect bar_rect(3, m_sliderPosition, m_imgHeight - 5, m_sliderHeight);
	gc->DrawRectangle(bar_rect);

	gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
	gc->DrawRectangleContour(bar_rect);

	gc->SetColor(axColor(0.45, 0.45, 0.45), 1.0);
	gc->DrawRectangleContour(axRect(4, m_sliderPosition + 1, m_imgHeight - 5 - 1, m_sliderHeight - 2));
}

//-----------------------------------------------------------------------------------
double axScrollBar::getZeroToOneValue()
{
	return (m_sliderPosition - m_imgHeight) / (GetSize().y - m_sliderHeight - m_imgHeight - 4);  
}


void axScrollBar::setInputInfo(int frameSize, int panelSize, int topDelta)
{
	m_inputFrameSize = frameSize;
	m_inputPanelSize = panelSize;
	m_inputTopDecay = topDelta;

	recalculateInputValue();
	Update();
}

void axScrollBar::setInputTopDelta(int y)
{
	m_inputTopDecay = y;

	recalculateInputValue();
	Update();
}

void axScrollBar::setInputFrameHeight(int y)
{
	m_inputFrameSize = y;
	
	recalculateInputValue();
	Update();
}

void axScrollBar::setInputPanelHeight(int y)
{
	m_inputPanelSize = y;

	recalculateInputValue();
	Update();
}

double axScrollBar::getSliderValue() const
{
	return m_value;
}

inline void axScrollBar::recalculateInputValue()
{
	double diff = double(m_inputPanelSize) - m_inputFrameSize;
	
	if(diff <= 0) 
	{
		m_sliderHeight = m_sliderMaxHeight;
		m_sliderPosition = m_imgHeight;	
		m_sliderValue = 0;
		m_value = 0;
	}
	else
	{  
		m_sliderHeight = (m_inputFrameSize / double(m_inputPanelSize)) * m_sliderMaxHeight;

		m_sliderPosition = m_imgHeight + m_sliderValue * m_sliderMaxHeight;

		if(m_sliderPosition + m_sliderHeight > GetRect().size.y - 14 - 1)
			m_sliderPosition = GetRect().size.y - 14 - 1 - m_sliderHeight;

		if(m_sliderPosition < m_imgHeight)
			m_sliderPosition = m_imgHeight;

		m_sliderValue = (m_sliderPosition - m_imgHeight) / (m_sliderMaxHeight);
		
		if(m_sliderHeight < m_sliderMaxHeight)
		{
			double d = (m_sliderPosition - m_imgHeight) / (m_sliderMaxHeight - m_sliderHeight);
			m_value = ceil(d * (m_inputPanelSize - m_inputFrameSize));
		}
		else m_value = 0;
	}
}


void axScrollBar::setSliderPositionZeroToOne(const double& value)
{
	double v = value;
	if( v < 0 ) v = 0;
	if( v > 1 ) v = 1;

	m_sliderPosition = v * (GetSize().y - m_sliderHeight - (2 * m_imgHeight)) + m_imgHeight;

	// If m_sliderPosition reach bottom, clip m_sliderPosition.
	if(m_sliderPosition + m_sliderHeight > GetRect().size.y - 14 - 1)
		m_sliderPosition = GetRect().size.y - 14 - 1 - m_sliderHeight;

	// Clip top m_sliderPosition .
	if(m_sliderPosition < m_imgHeight)
		m_sliderPosition = m_imgHeight;

	// Slider position ratio.
	m_sliderValue = (m_sliderPosition - m_imgHeight) / (m_sliderMaxHeight);
	
	double d = (m_sliderPosition - m_imgHeight) / (m_sliderMaxHeight - m_sliderHeight);
	m_value = ceil(d * (m_inputPanelSize - m_inputFrameSize));
	
	Update();
}

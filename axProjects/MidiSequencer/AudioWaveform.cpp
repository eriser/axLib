#include "AudioWaveform.h"

/*******************************************************************************//**
 * axWaveform.
 **********************************************************************************/
axWaveform::axWaveform(axApp* app, axWindow* parent, const axRect& rect,
                       const axWaveformEvents& events,
                       const axWaveformInfo& waveInfo):
                       axPanel(app, parent, rect),
                        // Members.
                        m_info(waveInfo)
                        // m_eventID( events )
{
    // SetCustomPaint( true );

    m_buffer = new axAudioBuffer("ohat.wav");

    // m_buffer = nullptr;
    // m_lineParams = nullptr;
}

void axWaveform::OnPaint()
{
    BlockDrawing(GetParent()->GetAbsoluteRect());

    axGC* gct = GetGC();
    axGC& gc = *gct;
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);

    // axGC gc( GetBackBuffer() );
    axSize size = rect.size;
    int middle_y = size.y * 0.5;
    
    gc.SetColor( m_info.background );
    gc.DrawRectangle( axRect(0, 0, size.x, size.y) );

    gc.SetColor( m_info.contour );
    gc.DrawRectangleContour( axRect(0, 0, size.x, size.y) );

    // gc.SetLineWidth( 1 );

    if( m_buffer )
    {
        //cout << "axWaveform :: Drawing buffer" << endl;

        axBufferInfo b_info = m_buffer->GetBufferInfo();
        axFloat* buffer = m_buffer->GetBuffer();

        // if( m_lineParams )
        // {
        //     long newNbFrame = m_lineParams->rSample - m_lineParams->lSample;
        //     long samples_per_line = newNbFrame / double( size.x - 2 );

        //     cout << "Frames per lines : " << samples_per_line << endl;

        //     axFloat v_min = 100, v_max = -100;

        //     gc.SetColor( m_info.lines );

        //     // Draw Lines.
        //     for( int i = 0; i < m_lineParams->nbLines; ++i )
        //     {
        //         int x = m_lineParams->linePos[i];
        //         gc.DrawLine( axPoint( x, 1 ), axPoint( x, size.y - 2) );
        //     }

        //     gc.SetColor( m_info.waveform );

        //     //for( int i = m_lineParams->lSample * 2, x = 1; i < m_lineParams->lSample * b_info.channels +
        //     //                                        newNbFrame * b_info.channels; i += b_info.channels )
        //     //int start_at = m_lineParams->lSample * b_info.channels,
        //     //    until = m_lineParams->rSample * b_info.channels;

        //     //for( int i = start_at, x = 1; i < until; i += b_info.channels )

        //     // Too complicated for nothing.
        //     for( int j = m_lineParams->lSample, x =1 , i = m_lineParams->lSample * b_info.channels;
        //          j < m_lineParams->rSample; i += 2, ++j )
        //     {
        //         // Left Channels.
        //         if( buffer[i] < v_min ) v_min = buffer[i];
        //         else if( buffer[i] > v_max) v_max = buffer[i];

        //         if( !( i % samples_per_line ) )
        //         {
        //             if( v_max < 0.0 ) v_max = 0.0;
        //             if( v_min > 0.0 ) v_min = 0.0;

        //             int y_min = middle_y - v_min * 0.9 * middle_y;
        //             int y_max = middle_y - v_max * 0.9 * middle_y;

        //             gc.DrawLine( axPoint( x, y_min ), axPoint( x, y_max ));

        //             v_min = 100; v_max = -100;
        //             ++x;
        //         }
        //     }
        // }

        // else
        // {
            int samples_per_line = b_info.frames / ( size.x - 2 );
            // cout << "Sample per line : " << samples_per_line << endl;
            axFloat v_min = 100, v_max = -100;

            gc.SetColor( m_info.waveform );
            for( int i = 0, x = 1; i < b_info.frames * b_info.channels; i += 2 )
            {
                // Left Channels.
                if( buffer[i] < v_min ) v_min = buffer[i];
                else if( buffer[i] > v_max) v_max = buffer[i];

                if( !( i % samples_per_line ) )
                {
                    if( v_max < 0.0 ) v_max = 0.0;
                    if( v_min > 0.0 ) v_min = 0.0;

                    int y_min = middle_y - v_min * 0.9 * middle_y;
                    int y_max = middle_y - v_max * 0.9 * middle_y;

                    gc.DrawLine( axPoint( x, y_min ), axPoint( x, y_max ));

                    v_min = 100;
                    v_max = -100;
                    x++;
                }
            }
        // }
    }

    // Draw horizontal middle line.
    gc.SetColor( m_info.lines );
    // gc.SetLineWidth( 1 );
    gc.DrawLine( axPoint( 1, middle_y ), axPoint( size.x - 2, middle_y ) );


    UnBlockDrawing();
    // FlipScreen( gc );
}

// //************************************************************************************************
// // axWaveformTime.
// //************************************************************************************************
// axWaveformTime::axWaveformTime( axSTANDARD_WIDGET_PARAMS( axWaveformTime ),
//                                 axAudioBuffer* audio_buffer ):
//                                 axSTANDARD_WIDGET_PARAMS_CONSTRUCT,
//                                 m_buffer( audio_buffer )
// {
//     SetCustomPaint( true );

//     nbFrames = newNbFrames = m_buffer->GetBufferInfo().frames;
//     SetYZoom(1);
//     xZoom = 1;
//     yMouseRatio = yPast = 1;
//     mouseDirection = NONE;
//     xDecay = 0;

//     setLeftSample(0);
//     setRightSample( nbFrames );

//     PrepareLines( rect.size );

// }

// /// @todo This function is way too slow.
// ///       Find out why the second algorithm doesn't work.
// void axWaveformTime::PrepareLines(axSize size)
// {
//     double totalTime = m_rSample - m_lSample;
//     double k = 1.0 / totalTime * size.x;

//     double time = SampleToTime( totalTime, 44100 );

//     int modulo;

//     if( time < 0.03125 )     modulo = 44100.0 / 512;
//     else if( time < 0.0625 ) modulo = 44100.0 / 256;
//     else if( time < 0.125 ) modulo = 44100.0 / 128;
//     else if( time < 0.25 )  modulo = 44100.0 / 64;
//     else if( time < 0.5 )   modulo = 44100.0 / 32;
//     else if( time < 1 )     modulo = 44100.0 / 16;
//     else if( time < 2 )     modulo = 44100.0 / 8;
//     else if( time < 5 )     modulo = 44100.0 / 4;
//     else if( time < 10 )    modulo = 44100.0 / 2.0;
//     else if( time < 20 )    modulo = 44100;
//     else if( time < 40 )    modulo = 44100 * 2;
//     else if( time < 80 )    modulo = 44100 * 4;
//     else if( time < 160 )   modulo = 44100 * 8;

//     int j = 0;
//     /// @todo It might be faster to do the if before the loop.
//     for( int i = m_lSample; i < m_rSample; ++i )
//     {
//         if( !( i % modulo ) )
//         {
//             double x = (i - m_lSample) * k;
//             linesParam.linePos[j] = x;

//             double t = SampleToTime( i );

//             if ( modulo < 44100.0 / 64 )
//                 linesParam.timeTxt[j] = axFloatToString( t, 5 );

//             else if( modulo < 44100 * 0.5 )
//                 linesParam.timeTxt[j] = axFloatToString( t, 4 );

//             else
//                 linesParam.timeTxt[j] = axFloatToString( t, 3 );

//             ++j;
//         }
//     }

//     linesParam.nbLines = j;

//     /// @todo Why this doesn't work ??? This should be faster than
//     ///       the other one because it doesn't go thru all samples between
//     ///       m_lSample and m_rSample: ( m_rSample - m_lSample ) iteration.
//     //    double nSample = m_rSample - m_lSample;
//     //    double time = SampleToTime( nSample );

//     //    // Find how many lines to draw depending on time lenght.
//     //    /// @todo Should also be a function of size.x to draw less
//     //    ///       lines if size.x is very small.
//     //    int sample_interval;

//     //    if( time < 0.25 )       sample_interval = 44100.0 / 64;
//     //    else if( time < 0.5 )   sample_interval = 44100.0 / 32;
//     //    else if( time < 1 )     sample_interval = 44100.0 / 16;
//     //    else if( time < 2 )     sample_interval = 44100.0 / 8;
//     //    else if( time < 5 )     sample_interval = 44100.0 / 4;
//     //    else if( time < 10 )    sample_interval = 44100.0 / 2.0; // Every half second.
//     //    else if( time < 20 )    sample_interval = 44100.0;       // Every second.
//     //    else if( time < 40 )    sample_interval = 44100 * 2;
//     //    else if( time < 80 )    sample_interval = 44100 * 4;
//     //    else if( time < 160 )   sample_interval = 44100 * 8;

//     //    // Find first line sample position.
//     //    long first_line_sample = 0;
//     //    for(int i = m_lSample; i < m_rSample; ++i )
//     //    {
//     //        if( !( i % sample_interval ) )
//     //        {
//     //            first_line_sample = i;
//     //            break;
//     //        }
//     //    }

//     //    // Find how many line there is to draw.
//     //    double ratio = 1.0 / nSample * size.x;
//     //    int nLines = 0;


//     //    /// @todo Make shure nLines doesn't exceed linesParam size.
//     //    for( int i = first_line_sample; i < m_rSample; i += sample_interval )
//     //    {
//     //        double x = (i - m_lSample) * ratio;
//     //        linesParam.linePos[nLines] = x;

//     //        double t = SampleToTime( i );

//     //        if ( sample_interval < 44100.0 / 64 )
//     //            linesParam.timeTxt[nLines] = axFloatToString( t, 6 );

//     //        else if( sample_interval < 44100 * 0.5 )
//     //            linesParam.timeTxt[nLines] = axFloatToString( t, 4 );

//     //        else
//     //            linesParam.timeTxt[nLines] = axFloatToString( t, 3 );

//     //        ++nLines;
//     //    }

//     //    linesParam.nbLines = nLines;
// }

// t__linesParam* axWaveformTime::getLinesParam()
// {
//     return &linesParam;
// }


// double axWaveformTime::getLeftSample()
// {
//     return xDecay;
// }

// double axWaveformTime::getRightSample()
// {
//     return xDecay + newNbFrames;
// }

// double axWaveformTime::getNewNbFrames()
// {
//     return newNbFrames;
// }

// double axWaveformTime::getClickSample()
// {
//     return s1;
// }
// double axWaveformTime::getNbFrames()
// {
//     return nbFrames;
// }

// void axWaveformTime::setLeftSample(double x)
// {
//     m_lSample = x;
// }

// int axWaveformTime::getClickLine()
// {
//     return clickLine;
// }

// void axWaveformTime::setClickSample(double x)
// {
//     m_cSample = x;
// }

// void axWaveformTime::setRightSample(double x)
// {
//     m_rSample = x;
// }
// void axWaveformTime::OnMouseLeftDown()
// {
//     axSize size = GetSize();
//     clickPts = GetLeftClickPosition();

//     s1 = xDecay + clickPts.x * newNbFrames / double( size.x );

//     GrabMouse();
// }

// void axWaveformTime::OnMouseLeftUp()
// {
//     if( IsGrabbed() )
//     {
//         UnGrabMouse();
//         mouseDirection = NONE;
//     }
// }

// void axWaveformTime::SetYZoom(double f)
// {
//     axCLIP( f, 0.1, 5 );
// //    if(f < 0.1) f = 0.1;
// //    if(f > 5)   f = 5;

//     zoomY = f;
//     yZoom = f * GetSize().y * 0.5;
// }

// /// @todo Way too complex for nothing, if not at least encapsulate complexity.
// void axWaveformTime::OnLeftDragging()
// {
//     axSize size = GetSize();

//     double v = - GetDeltaFromUpDownDirectionPoint().y / 3000.0;
//     xZoom += v;
//     axCLIP( xZoom, 0.0, 1.0 );
//     mousePos = GetMousePosition();

//     /// @todo find a better function.
//     double f = pow( xZoom, 6 ); // To simulate linearity.

//     axCLIP( f, 0.2 * (size.x - 1) / nbFrames, 1 );

//     newNbFrames = f * nbFrames;
//     xDecay = s1 - clickPts.x * newNbFrames / (size.x * 1.0);

//     // Motion in x.
//     s2 = ((clickPts.x - mousePos.x) * newNbFrames / size.x);
//     xDecay += s2;

//     axCLIP( xDecay, 0, nbFrames - newNbFrames );

//     setLeftSample( xDecay );
//     setRightSample( xDecay + newNbFrames );

//     //CLICK LINE
//     int x;
//     if( xDecay < 0.2 * ( size.x - 1 ) )
//         x = s1 / newNbFrames * size.x * 1.0;

//     else if( xDecay == nbFrames - newNbFrames )
//         x = (s1 - xDecay) / newNbFrames * size.x * 1.0;

//     else
//         x = clickPts.x - (clickPts.x - mousePos.x);

//     clickLine = x;

//     PrepareLines( size );

//     GetParent()->TriggerEvent( m_eventsID.motion );

//     Update();
// }

// void axWaveformTime::OnPaint()
// {
//     axGC gc( GetBackBuffer() );
//     axSize size = GetSize();

//     gc.SetColor( axColor("#888888") );
//     gc.DrawRectangle( axRect( axPoint( 0, 0 ), size ) );

//     gc.SetColor( axColor("#000000") );
//     gc.SetLineWidth( 1 );

//     //DRAW LINES
//     for( int i = 0; i < linesParam.nbLines; ++i )
//     {
//         int x = linesParam.linePos[i];
//         gc.DrawLine( axPoint( x, size.y ),
//                      axPoint( x, size.y - 5 ) );

// //        gc.DrawText( linesParam.timeTxt[i],
// //                     axPoint( x + 3, 2 ),
// //                     axColor("#888888").GetColorRGB(), "7" );

//         gc.DrawString( linesParam.timeTxt[i], axPoint( x + 3, 12 ) );
//     }

//     gc.DrawRectangleContour( axRect( axPoint( 0, 0 ), size - axSize( 1, 0 ) ) );

//     FlipScreen( gc );

// }

// //************************************************************************************************
// // WAVEFORM_PANEL
// //************************************************************************************************
// A_WaveformPanel::A_WaveformPanel( axApp* app,
//                                   axWindow* parent,
//                                   const axID& id,
//                                   const axWaveformEvents& events,
//                                   const axWaveformInfo& waveInfo,
//                                   const axRect& rect,
//                                   axAudioBuffer* buffer ):
//                                   axWindow( app, parent, id, rect )
// {
//     SetCustomPaint( true );

//     m_buffer = buffer;
//     InitBuffer();

//     InitParam();
// }

// //void A_WaveformPanel::setMouseSelected(bool mSelected)
// //{
// //    mouseSelected = mSelected;
// //}
// void A_WaveformPanel::setClickLine(int cLine)
// {
//     clickLine = cLine;
// }
// void A_WaveformPanel::setClickSample(double sample)
// {
//     s1 = sample;
// }
// void A_WaveformPanel::setLeftSample(double s)
// {
//     xDecay = s;
// }

// void A_WaveformPanel::setNewNbFrames(double s)
// {
//     newNbFrames = s;
// }
// void A_WaveformPanel::InitParam()
// {
//     SetYZoom(1);

//     xZoom = 1;
//     yMouseRatio = yPast = 1;
//     mouseDirection = NONE;
//     xDecay = 0;
//     xDeplacement = 1;
//     origine = 1;
// }

// void A_WaveformPanel::InitBuffer()
// {
//     buf = m_buffer->GetBuffer();
//     bufL = bufR = NULL;

//     newNbFrames = nbFrames = nbFramesAtClick = m_buffer->GetBufferInfo().frames;

//     if(bufL != NULL)
//     {
//         delete bufL;
//         bufL = new axFloat [int(nbFrames)];
//     }
//     else bufL = new axFloat [int(nbFrames)];

//     if(bufR != NULL)
//     {
//         delete bufR;
//         bufR = new axFloat [int(nbFrames)];
//     }
//     else bufR = new axFloat [int(nbFrames)];

//     //----------------------------------------
//     if( m_buffer->GetBufferInfo().channels == 1)
//     {
//         for(int i = 0; i < nbFrames; i ++)
//             bufL[i] = buf[i];
//     }

//     else if( m_buffer->GetBufferInfo().channels == 2)
//     {
//         for(int i = 0, j = 0; i < nbFrames * m_buffer->GetBufferInfo().channels; i += 2, j++)
//         {
//             bufL[j] = buf[i];
//             bufR[j] = buf[i + 1];
//         }
//     }
// }

// void A_WaveformPanel::OnZoomYIn()
// {
//     SetYZoom(zoomY + 0.2);
// }

// void A_WaveformPanel::OnZoomYOut()
// {
//     SetYZoom(zoomY - 0.2);
// }

// double A_WaveformPanel::getClickSamplePos()
// {
//     return s1;
// }
// double A_WaveformPanel::getLastSamplePos()
// {
//     return xDecay + newNbFrames;
// }

// double A_WaveformPanel::getFirstSamplePos()
// {
//     return xDecay;
// }

// void A_WaveformPanel::SetYZoom(double f)
// {
//     double zoom = f;

//     axCLIP( zoom, 0.1, 5.0 );

//     zoomY = zoom;

//     yZoom = zoom * GetSize().y * 0.5;

//     //Refresh();
// }

// void A_WaveformPanel::setLinesParam(t__linesParam* lp)
// {
//     linesParam = lp;
// }
// void A_WaveformPanel::setLeftLoopLine(int l)
// {
//     lLoop = l;
// }
// void A_WaveformPanel::setRightLoopLine(int r)
// {
//     rLoop = r;
// }
// void A_WaveformPanel::DrawBuffer( axGC& gc, axSize size, axFloat* b, double midPoint, double height)
// {
//     double r = (size.x - 1) / newNbFrames;
//     long samples_per_line = newNbFrames / double( size.x - 2 );
//     int midY = midPoint;
//     double min = 100, max = -100;

//     cout << "Sample per line : " << samples_per_line << endl;

//     gc.SetColor( axColor("#000000") );

//     if(r < 0.1)
//     {
//         for(int i = xDecay, x = 1; i < newNbFrames + xDecay; i++)
//         {
//             int p = x;
//             x = (i - xDecay) * r;

//             if( b[i] < min ) min = b[i];

//             else if( b[i] > max ) max = b[i];

//             if( p != x )
//             {
//                 if( max < 0 ) max = 0;
//                 if( min > 0 ) min = 0;

//                 gc.DrawLine( axPoint( x, midY - (min * yZoom * height) ),
//                              axPoint( x + r, midY - (max * yZoom * height) ) );

//                 min = 100; max = -100;
//             }
//         }
//     }

//     else
//     {
//         int j = 0;
//         for(int i = xDecay, x = 1; i < newNbFrames + xDecay; i++, x = (i - xDecay) * r)
//         {
//             gc.DrawLine( axPoint( x, midY - b[i] * yZoom * height),
//                          axPoint( x + r , midY - b[i + 1] * yZoom * height ) );
//         }
//     }

// }

// void A_WaveformPanel::OnPaint()
// {
//     axGC gc( GetBackBuffer() );

//     axSize size = GetSize();
//     gc.SetColor( axColor("#888888") );
//     gc.DrawRectangle( axRect(0, 0, size.x, size.y) );

//     gc.SetLineWidth( 1 );
//     gc.SetColor( axColor("#AAAAAA") );

//     //DRAW LINES
//      for(int i = 0; i < linesParam->nbLines; i++)
//     {
//         int x = linesParam->linePos[i];
//         gc.DrawLine( axPoint( x, 1 ), axPoint( x, size.y - 1 ) );
//     }

//     //DRAW WAVEFORM
//     if( m_buffer->GetBufferInfo().channels == 1)
//         DrawBuffer(gc, size, bufL, size.y * 0.5, 1);

//     else if(m_buffer->GetBufferInfo().channels == 2)
//     {
//         DrawBuffer(gc, size, bufL, size.y * 0.25, 0.5);
//         DrawBuffer(gc, size, bufR, size.y * 0.75, 0.5);
//     }

//     //--------------------------------------------------
// //    //DRAW LINE CLICK
// //    if(mouseSelected)
// //    {
// //        dc.SetPen(wxPen(wxColor(100, 0 ,0) , 1, wxSOLID));
// //        dc.DrawLine(clickLine, 1, clickLine, size.y - 1);
// //    }

//     //--------------------------------------------------
//     //DRAW LOOP
//     /*dc.SetPen(wxPen(wxColor(120, 120 ,120) , 1, wxSOLID));
//     dc.DrawLine(lLoop, 1, lLoop, size.y - 1);
//     dc.DrawLine(rLoop, 1, rLoop, size.y - 1);*/

//     gc.SetColor( axColor("#000000") );
//     gc.DrawRectangleContour( axRect(0, 0, size.x - 1, size.y - 1) );

//     FlipScreen( gc );
// }

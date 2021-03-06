/*
 Copyright (C) 2012 Gabor Papp

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cinder/Cinder.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "CiNI.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace mndl;

class NIBasicApp : public AppBasic
{
	public:
		void prepareSettings( Settings *settings );
		void setup();

		void update();
		void draw();

		void mouseUp( MouseEvent event );

	private:
		ni::OpenNI mNI;
		gl::Texture mColorTexture, mDepthTexture;
};

void NIBasicApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize( 1280, 480 );
}

void NIBasicApp::setup()
{
	ni::OpenNI::Options options;
	options.enableUserTracker( false );

	try
	{
		mNI = ni::OpenNI( ni::OpenNI::Device(), options );
	}
	catch ( ni::OpenNIExc &exc )
	{
		console() << "Could not open Kinect" << endl;
		quit();
	}

	mNI.start();
}

void NIBasicApp::update()
{
	if ( mNI.checkNewVideoFrame() )
		mColorTexture = mNI.getVideoImage();
	if ( mNI.checkNewDepthFrame() )
		mDepthTexture = mNI.getDepthImage();
}

void NIBasicApp::draw()
{
	gl::clear( Color::black() );
	gl::setMatricesWindow( getWindowWidth(), getWindowHeight() );

	if ( mDepthTexture )
		gl::draw( mDepthTexture );
	if ( mColorTexture )
		gl::draw( mColorTexture, Vec2i( 640, 0 ) );
}

void NIBasicApp::mouseUp( MouseEvent event )
{
    // toggle infrared video
    mNI.setVideoInfrared( !mNI.isVideoInfrared() );
}

CINDER_APP_BASIC( NIBasicApp, RendererGl() )


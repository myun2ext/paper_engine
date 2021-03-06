#ifndef __MYUN2_GITHUB_COM__PAPER_ENGINE__APP_HPP__
#define __MYUN2_GITHUB_COM__PAPER_ENGINE__APP_HPP__

#include "window_class.hpp"
#include "window.hpp"
#include "direct3d.hpp"
#include "d3ddev.hpp"
#include "text.hpp"
#include "sprite.hpp"
#include "input.hpp"
#include "dsound.hpp"
#include "game/value.hpp"
#include <string>

namespace myun2
{
	namespace paperengine
	{
		struct renderer_base {
		/*private:
			d3ddev d;
		public:
			renderer_base(d3ddev d_in) : d(d_in){}*/
			virtual void render(d3ddev &d) =0;
		};
		template <typename _Renderer>
		class app
		{
		public:
			window_class wc;
			window w;
			HWND hwnd;
			direct3d d3d;
			d3ddev d;
			dinput di;
			kb_input kb;
			mouse_input mouse;
			dsound snd;
			_Renderer r;

			app(const char* name, HINSTANCE hinstance, unsigned int width, unsigned int height)
				: wc((::std::string(name) + "__WindowClass__").c_str()),
				  w(wc.atom, name, width + 16, height + 38),
				  hwnd(w.hWnd),
				  d(d3d, hwnd, width, height),
				  di(hinstance),
				  kb(di, hwnd),
				  mouse(di, hwnd),
				  r(d, kb, mouse)
			{
				w.show();
			}
			int message_loop()
			{
				MSG msg;
				do
				{
					if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
						TranslateMessage( &msg );
						DispatchMessage( &msg );
					}
					else
					{
						kb.update();
						mouse.update();
						d.clear();
						d.begin();
						r.render();
						d.end();
						d.present();
					}
				}while(msg.message != WM_QUIT);
				return 0;
			}
		};
	}
}

#define RUN_PAPERENGINE(NAME, WIDTH, HEIGHT, RENDERER)	\
	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {	\
		::myun2::paperengine::app<RENDERER> a(NAME, hInstance, WIDTH, HEIGHT);	\
		return a.message_loop();	\
	}

#endif///__MYUN2_GITHUB_COM__PAPER_ENGINE__APP_HPP__

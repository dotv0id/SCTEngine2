#include <Input\Win32\SCTMouseWin32.h>

SCTMouseWin32(IDirectInput8 *inputDevice);
			~SCTMouseWin32();

			SCTReturn				Initialize();
			SCTReturn				Update();
			SCTReturn				Shutdown();

			bool					IsButtonUp(SCTMouseCode buttonID);
			bool					isButtonDown(SCTMouseCode buttonID);

			// Get, Set functions
			void					GetAbsolutePosition(float &x, float &y);
			float					GetRelativeX();
			float					GetRelativeY();
			float					GetRelativeZ();
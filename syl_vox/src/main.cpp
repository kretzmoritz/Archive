#include "window/creation/factory.h"
#include "window/creation/info.h"
#include "application/custom_events/sv_event.h"
#include "application/components/sv_temp_context.h"
#include "application/components/sv_context.h"
#include "application/components/sv_menu.h"
#include "application/sv_main.h"

int main(int argc, char** argv)
{
	Factory factory;
	
	factory.construct(Info("syl_temp", false, Info::WinShow::HIDE)); //2 stage OpenGL context creation
	factory.getWindow("syl_temp")->registerComponent<SVTempContext>();
	factory.getWindow("syl_temp")->destruct();

	factory.construct<SVEvent>(Info("syl_vox", true));
	factory.getWindow("syl_vox")->registerComponent<SVContext>();
	factory.getWindow("syl_vox")->registerComponent<SVMenu>();
	factory.getWindow("syl_vox")->registerComponent<SVMain>();

	factory.run();

	return 0;
}
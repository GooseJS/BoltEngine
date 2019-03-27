#include <Bolt.h>
#include <BoltEngine/voxel/Block.h>
#include <BoltEngine/BoltApplication.h>
#include <BoltEngine/render/imgui/BoltImGui.h>

namespace Bolt
{
	namespace Editor
	{
		class BlockAir : public Block
		{
		public:
			BlockAir(BlockID id) : Block(id) {}
		};
		
		class BlockDirt : public Block
		{
		public:
			BlockDirt(BlockID id) : Block(id) {} // TODO (Brendan): Move BlockNameGen into the block manager and seperate into modules / mods (bolt == base game)

			virtual bool shouldRender() override { return true; }
		};

		class BoltEditor : public BoltApplication
		{
		private:
		public:
			virtual void onStartup() override
			{
				ImGuiContext* imGuiContext = Bolt::BoltImGui::getInstance().init(getMainWindow());
				ImGui::SetCurrentContext(imGuiContext);
			}

			virtual void update() override
			{

			}
			
			virtual void draw(float updateDistance) override
			{
				glClear(GL_COLOR_BUFFER_BIT);

				Bolt::BoltImGui::getInstance().newFrame();

				ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
				{

				}

				static bool demoWindowShown = false;
				ImGui::ShowDemoWindow(&demoWindowShown);

				ImGui::End();

				Bolt::BoltImGui::getInstance().render();
			}

			virtual void onShutdown() override
			{

			}
		};
	}
}

int main()
{
	Bolt::Editor::BoltEditor editor;
	editor.main();
}
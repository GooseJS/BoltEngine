#include <Bolt.h>
#include <BoltEngine/voxel/Block.h>
#include <BoltEngine/BoltApplication.h>
#include <BoltEngine/voxel/BlockManager.h>
#include <BoltEngine/render/imgui/BoltImGui.h>
#include <BoltEngine/voxel/Chunk.h>

#include <BoltEngine/voxel/World.h>

namespace Bolt
{
	namespace Editor
	{
		class BlockAir : public Block
		{
		public:
			BlockAir() {}

			virtual const std::string getName() override { return "bolt::air"; }
		};
		
		class BlockDirt : public Block
		{
		public:
			BlockDirt(){} // TODO (Brendan): Move BlockNameGen into the block manager and seperate into modules / mods (bolt == base game)

			virtual const std::string getName() override { return "bolt::dirt"; }

			virtual bool shouldRender() override { return true; }
		};

		class BoltEditor : public BoltApplication
		{
		private:
			BlockAir* AIR_BLOCK;
			BlockDirt* DIRT_BLOCK;

			World world;
		public:
			BoltEditor()
			{

			}

			virtual void onStartup() override
			{
				ImGuiContext* imGuiContext = Bolt::BoltImGui::getInstance().init(getMainWindow());
				ImGui::SetCurrentContext(imGuiContext);

				AIR_BLOCK = dynamic_cast<BlockAir*>(&BlockManager::getInstance().registerBlock(new BlockAir()));
				DIRT_BLOCK = dynamic_cast<BlockDirt*>(&BlockManager::getInstance().registerBlock(new BlockDirt()));

				BOLT_INFO("Air ID: {} | should render: {}", AIR_BLOCK->getID(), AIR_BLOCK->shouldRender());
				BOLT_INFO("Dirt ID: {} | should render: {}", DIRT_BLOCK->getID(), DIRT_BLOCK->shouldRender());

				world.setBlockAt(BlockPos(65, 75, 36), *DIRT_BLOCK);
				BOLT_INFO("Block at 10, 10, 10: {}", world.getBlockAt(BlockPos(65, 75, 36)).getName());
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
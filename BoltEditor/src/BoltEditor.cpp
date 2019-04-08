#include <Bolt.h>
#include <BoltEngine/GL.h>
#include <BoltEngine/voxel/Block.h>
#include <BoltEngine/BoltApplication.h>
#include <BoltEngine/voxel/BlockManager.h>
#include <BoltEngine/render/imgui/BoltImGui.h>
#include <BoltEngine/voxel/Chunk.h>
#include <BoltEngine/voxel/World.h>
#include <BoltEngine/voxel/WorldRenderer.h>
#include <BoltEngine/event/EventSystem.h>
#include <BoltEngine/voxel/Player.h>

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
			BlockDirt() {} // TODO (Brendan): Move BlockNameGen into the block manager and seperate into modules / mods (bolt == base game)

			virtual const std::string getName() override { return "bolt::dirt"; }

			virtual bool shouldRender() override { return true; }
		};

		class BoltEditor : public BoltApplication
		{
		private:
			BlockAir* AIR_BLOCK;
			BlockDirt* DIRT_BLOCK;

			World world;
			WorldRenderer worldRenderer;

			Player player;

			GLuint vaoID;
			GLuint elementArrayID;
			GLuint indexArrayID;
			Shader shader;
		public:
			BoltEditor()
			{

			}

			virtual void onStartup() override
			{
				//shader.initFromFile("shader.vert", "shader.frag");

				EventSystem::getInstance().addListener<EventMouseMove>(BOLT_SUBSCRIBE_EVENT(&BoltEditor::onMouseMove));

				getMainWindow().disableCursor();

				ImGuiContext* imGuiContext = Bolt::BoltImGui::getInstance().init(getMainWindow());
				ImGui::SetCurrentContext(imGuiContext);

				BlockManager::getInstance().requireTexture("dirt.png");
				BlockManager::getInstance().requireTexture("grass-top.png");
				BlockManager::getInstance().requireTexture("grass-side.png");
				BlockManager::getInstance().createTexture();

				worldRenderer.setWorldRenderCam(&player.getPlayerCamera());

				AIR_BLOCK = dynamic_cast<BlockAir*>(&BlockManager::getInstance().registerBlock(new BlockAir()));
				DIRT_BLOCK = dynamic_cast<BlockDirt*>(&BlockManager::getInstance().registerBlock(new BlockDirt()));

				BOLT_INFO("Air ID: {} | should render: {}", AIR_BLOCK->getID(), AIR_BLOCK->shouldRender());
				BOLT_INFO("Dirt ID: {} | should render: {}", DIRT_BLOCK->getID(), DIRT_BLOCK->shouldRender());


				world.setBlockAt(BlockPos(32, 75, 36), *DIRT_BLOCK);
				BOLT_INFO("Block at 10, 10, 10: {}", world.getBlockAt(BlockPos(32, 75, 36)).getName());

				worldRenderer.initRenderer();

				for (int x = 0; x < 16; x++)
				{
					for (int y = 0; y < 16; y++)
					{
						for (int z = 0; z < 16; z++)
						{
							if (z % 2 == 0)
								world.setBlockAt(BlockPos(x, y, z), *DIRT_BLOCK);
						}
					}
				}
				for (int x = 0; x < 16; x++)
				{
					for (int y = 16; y < 32; y++)
					{
						for (int z = 0; z < 16; z++)
						{
							if (x % 2 == 0)
								world.setBlockAt(BlockPos(x, y, z), *DIRT_BLOCK);
						}
					}
				}

				worldRenderer.createChunkMesh(world.getChunkAt(BlockPos(5, 5, 5)));
				worldRenderer.createChunkMesh(world.getChunkAt(BlockPos(32, 75, 36)));
			}

			virtual void update() override
			{
				updateCamera();
			}
			
			virtual void draw(float updateDistance) override
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				Bolt::BoltImGui::getInstance().newFrame();

				ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
				{
					ImGui::Text("Player X: %.2f", player.getX());
					ImGui::Text("Player Z: %.2f", player.getZ());
				}
				ImGui::End();

				worldRenderer.renderWorld(&world);

				static bool demoWindowShown = false;
				//ImGui::ShowDemoWindow(&demoWindowShown);

				Bolt::BoltImGui::getInstance().render();			
			}

			virtual void onShutdown() override
			{

			}

			void onMouseMove(EventMouseMove mouseMoveEvent)
			{
				player.handleRotationInput(mouseMoveEvent.deltaX(), mouseMoveEvent.deltaY());
			}

			void updateCamera()
			{
				PlayerMovement playerMovement{};

				playerMovement.moveForwardPressed = getMainWindow().isKeyPressed(GLFW_KEY_W);
				playerMovement.moveBackPressed = getMainWindow().isKeyPressed(GLFW_KEY_S);
				playerMovement.moveLeftPressed = getMainWindow().isKeyPressed(GLFW_KEY_A);
				playerMovement.moveRightPressed = getMainWindow().isKeyPressed(GLFW_KEY_D);
				playerMovement.moveUpPressed = getMainWindow().isKeyPressed(GLFW_KEY_Q);
				playerMovement.moveDownPressed = getMainWindow().isKeyPressed(GLFW_KEY_Z);

				player.handleMovementInput(playerMovement);
			}
		};
	}
}

int main()
{
	Bolt::Editor::BoltEditor editor;
	editor.main();
}
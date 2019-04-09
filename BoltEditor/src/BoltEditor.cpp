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

		class BoltEditor : public BoltApplication
		{
		private:
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
				EventSystem::getInstance().addListener<EventMouseMove>(BOLT_SUBSCRIBE_EVENT(&BoltEditor::onMouseMove));

				getMainWindow().disableCursor();

				ImGuiContext* imGuiContext = Bolt::BoltImGui::getInstance().init(getMainWindow());
				ImGui::SetCurrentContext(imGuiContext);

				BlockManager::getInstance().requireTexture("dirt.png");
				BlockManager::getInstance().requireTexture("grass-top.png");
				BlockManager::getInstance().requireTexture("grass-side.png");
				BlockManager::getInstance().createTexture();

				worldRenderer.setWorldRenderCam(&player.getPlayerCamera());

				Block& airBlock = BlockManager::getInstance().registerBlock("bolt", "air");
				Block& dirtBlock = BlockManager::getInstance().registerBlock("bolt", "dirt");
				Block& grassBlock = BlockManager::getInstance().registerBlock("bolt", "grass");

				worldRenderer.initRenderer();

				for (int x = 0; x < 16; x++)
				{
					for (int y = 0; y < 16; y++)
					{
						for (int z = 0; z < 16; z++)
						{
							if (z % 2 == 0)
								world.setBlockAt(BlockPos(x, y, z), dirtBlock);
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
								world.setBlockAt(BlockPos(x, y, z), grassBlock);
						}
					}
				}

				worldRenderer.createChunkMesh(world.getChunkAt(BlockPos(5, 5, 5)));
				worldRenderer.createChunkMesh(world.getChunkAt(BlockPos(32, 75, 36)));
			}

			virtual void update() override
			{
				updateCamera();

				if (getMainWindow().isKeyPressed(GLFW_KEY_SPACE))
				{
					world.setBlockAt(BlockPos(player.getX(), player.getY() - 1, player.getZ()), BlockManager::getInstance().getBlock(0));
				}
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

				worldRenderer.buildChunks(&world);
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
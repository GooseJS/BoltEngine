//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

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
#include <BoltEngine/util/BoltProfiler.h>
#include <BoltEngine/util/TaskScheduler.h>
#include <BoltEngine/render/DebugRenderer.h>
#include <BoltEngine/voxel/EarthWorldGenerator.h>

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

			BoltDebugDrawInterface debugRenderer;

			EarthWorldGenerator generator;
		public:
			BoltEditor()
			{

			}

			virtual void onStartup() override
			{
				debugRenderer.setup();
				debugRenderer.setRenderCam(&player.getPlayerCamera());
				dd::initialize(&debugRenderer);

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

				generator.setup();

				generator.generateChunk(world.getChunkColumnAt(ChunkPos(0, 0, 0)));
				generator.generateChunk(world.getChunkColumnAt(ChunkPos(0, 0, 1)));
				generator.generateChunk(world.getChunkColumnAt(ChunkPos(1, 0, 1)));
				generator.generateChunk(world.getChunkColumnAt(ChunkPos(1, 0, 0)));

				world.setWorldGenerator(&generator);
				
				worldRenderer.initialBuild(&world);

				TaskScheduler::getInstance().startTaskThread();
			}

			virtual void update() override
			{
				if (getMainWindow().isKeyPressed(GLFW_KEY_X))
				{
					world.setBlockAt(BlockPos(player.getX(), player.getY(), player.getZ()), BlockManager::getInstance().getBlock(0));
				}

				if (getMainWindow().isKeyPressed(GLFW_KEY_T))
					getMainWindow().setShouldClose();

				player.checkCollisions(&world);

				world.updateRenderChunksFromPos(BlockPos(player.getX(), player.getY(), player.getZ()), &worldRenderer);

				TaskScheduler::getInstance().update();
			}
			
			virtual void draw(float updateDistance) override
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				updateCamera();

				Bolt::BoltImGui::getInstance().newFrame();

				ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
				{
					ImGui::Text("Player X: %.2f", player.getX());
					ImGui::Text("Player Y: %.2f", player.getY());
					ImGui::Text("Player Z: %.2f", player.getZ());
				}
				ImGui::End();

				player.updatePos();

				worldRenderer.checkForRebuildChunks(&world);
				worldRenderer.renderWorld(&world);

				player.debugDraw();

				static bool demoWindowShown = false;
				//ImGui::ShowDemoWindow(&demoWindowShown);

				Bolt::BoltImGui::getInstance().render();

				//Bolt::BoltProfiler::getInstance().newFrame();

				dd::flush();
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
				playerMovement.jumpPressed = getMainWindow().isKeyPressed(GLFW_KEY_SPACE);

				player.handleMovementInput(playerMovement);
			}
		};
	}
}

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Bolt::Editor::BoltEditor editor;
	editor.main();
}
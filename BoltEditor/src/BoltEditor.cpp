#include <Bolt.h>
#include <BoltEngine/BoltApplication.h>
#include <BoltEngine/render/imgui/BoltImGui.h>
#include <BoltEngine/gameObjects/GameObjectManager.h>

#include <BoltEngine/gameObjects/Actor.h>

namespace Bolt
{
	namespace Editor
	{
		class BoltEditor : public BoltApplication
		{
		private:
			void displayEntitySpawnWindow()
			{
				static const char* names[2] = { "Actor", "Player" };
				for (int n = 0; n < IM_ARRAYSIZE(names); n++)
				{
					ImGui::PushID(n);
					//if ((n % 3) != 0)
					//	ImGui::SameLine();
					ImGui::Button(names[n], ImVec2(60, 60));

					// Our buttons are both drag sources and drag targets here!
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));        // Set payload to carry the index of our item (could be anything)
						ImGui::Text("Spawn %s", names[n]);
						ImGui::EndDragDropSource();
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
						{
							//IM_ASSERT(payload->DataSize == sizeof(int));
							//int payload_n = *(const int*)payload->Data;
							//if (mode == Mode_Copy)
							//{
							//	names[n] = names[payload_n];
							//}
							//if (mode == Mode_Move)
							//{
							//	names[n] = names[payload_n];
							//	names[payload_n] = "";
							//}
							//if (mode == Mode_Swap)
							//{
							//	const char* tmp = names[n];
							//	names[n] = names[payload_n];
							//	names[payload_n] = tmp;
							//}
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::PopID();
				}
			}
		public:
			virtual void onStartup() override
			{
				ImGuiContext* imGuiContext = Bolt::BoltImGui::getInstance().init(getMainWindow());
				ImGui::SetCurrentContext(imGuiContext);

				GameObjectManager::getInstance().addGameObject(new Actor());
				GameObjectManager::getInstance().addGameObject(new Actor());
			}

			virtual void update() override
			{
			}
			
			virtual void draw(float updateDistance) override
			{
				glClear(GL_COLOR_BUFFER_BIT);

				Bolt::BoltImGui::getInstance().newFrame();

				ImGui::Begin("GameObjects", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
				{
					auto gameObjects = GameObjectManager::getInstance().getGameObjects();
					for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
					{
						ImGui::Text("%s", (*iter)[0].getName().c_str());
					}
				}

				static bool demoWindowShown = true;
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
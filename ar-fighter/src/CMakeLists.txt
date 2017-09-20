
include(src/AI/CMakeLists.txt)
include(src/Game_Objects/CMakeLists.txt)

list(APPEND AR_FIGHTER_SRC
	"src/ARFighter.cpp"
	"src/FightSceneLogic.cpp"
	"src/MainMenuSceneLogic.cpp"
	"src/SettingsSceneLogic.cpp"
)
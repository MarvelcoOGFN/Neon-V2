#pragma once

namespace Events
{
	struct Event
	{
		std::string Loader;
		std::string Scripting;
		std::vector<std::pair<bool, std::string>> Functions; // bIsLoaderFunc, FuncPath
		FFortniteVersion Version;
	
		std::string LoaderFunc = "";
	};

	static inline std::vector<Event> EventsArray =
	{
		Event
		(
			"",
			"/Game/Athena/Maps/Test/Events/BP_GeodeScripting.BP_GeodeScripting_C",
			{
					{
						false,
					   "/Game/Athena/Maps/Test/Events/BP_GeodeScripting.BP_GeodeScripting_C.LaunchSequence"
					}
			},
			FFortniteVersion(4,5,0)
		),
		Event
		(
			"/Game/Athena/Prototype/Blueprints/Cube/CUBE.CUBE_C",
			"/Game/Athena/Events/BP_Athena_Event_Components.BP_Athena_Event_Components_C",
			{
				{
					false,
					"/Game/Athena/Events/BP_Athena_Event_Components.BP_Athena_Event_Components_C.Final"
				},
				{
					true,
					"/Game/Athena/Prototype/Blueprints/Cube/CUBE.CUBE_C.Final"
				}
			},
			FFortniteVersion(5,30,0)
		),
		Event
		(
			"/Game/Athena/Prototype/Blueprints/Island/BP_Butterfly.BP_Butterfly_C",
			"",
			{
				{
					true,
					"/Game/Athena/Prototype/Blueprints/Island/BP_Butterfly.BP_Butterfly_C.ButterflySequence"
				}
			},
			FFortniteVersion(6,21,0),
			"/Game/Athena/Prototype/Blueprints/Island/BP_Butterfly.BP_Butterfly_C.LoadButterflySublevel"
		),
		Event(
			"",
			"/Game/Athena/Prototype/Blueprints/White/BP_SnowScripting.BP_SnowScripting_C",
			{
				{
					false,
					"/Game/Athena/Prototype/Blueprints/White/BP_SnowScripting.BP_SnowScripting_C.FinalSequence"
				}
			},
			FFortniteVersion(8, 51, 0),
			"/Game/Athena/Prototype/Blueprints/White/BP_SnowScripting.BP_SnowScripting_C.LoadSnowLevel"
		),
		Event
		(
			"/Game/Athena/Prototype/Blueprints/Cattus/BP_CattusDoggus_Scripting.BP_CattusDoggus_Scripting_C",
			"",
			{
				{
					true,
					"/Game/Athena/Prototype/Blueprints/Cattus/BP_CattusDoggus_Scripting.BP_CattusDoggus_Scripting_C.OnReady_C11CA7624A74FBAEC54753A3C2BD4506"
				},
				{
					true,
					"/Game/Athena/Prototype/Blueprints/Cattus/BP_CattusDoggus_Scripting.BP_CattusDoggus_Scripting_C.startevent"
				}
			},
			FFortniteVersion(9,41,0),
			"/Game/Athena/Prototype/Blueprints/Cattus/BP_CattusDoggus_Scripting.BP_CattusDoggus_Scripting_C.LoadCattusLevel"
		)
	};

	void StartEvent();
}
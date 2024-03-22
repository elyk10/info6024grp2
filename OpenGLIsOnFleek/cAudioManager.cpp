#include "cAudioManager.h"

	void GLMToFMOD(const glm::vec3& in, FMOD_VECTOR& out)
	{
		out.x = in.x;
		out.y = in.y;
		out.z = in.z;
	}
	void FMODToGLM(const FMOD_VECTOR& in, glm::vec3& out)
	{
		out.x = in.x;
		out.y = in.y;
		out.z = in.z;
	}

	const int MAX_CHANNELS = 20;

	void CheckError(FMOD_RESULT result, const char* file, int line)
	{
		if (result != FMOD_OK)
		{
			printf("FMOD Error [%d]: '%s' at %d\n", static_cast<int>(result), file, line);
		}
	}

	AudioManager::AudioManager()
	{
		// Usually do nothing that requires code execution
		// Declare initial values for everything
		// Utilize Initialize function instead
	}

	AudioManager::~AudioManager()
	{
		// Typically just for notifying things that this is being destroyed.
		// Utilize the Destroy function instead
	}

	void AudioManager::Initialize()
	{
		//new HighPassFilter();

		if (m_Initialized)
			return;

		FMOD_RESULT result;
		result = FMOD::System_Create(&m_System);
		if (result != FMOD_OK)
		{
			printf("Failed to create the FMOD System!\n");
			return;
		}

		result = m_System->init(MAX_CHANNELS, FMOD_INIT_NORMAL | FMOD_INIT_PROFILE_ENABLE, nullptr);
		if (result != FMOD_OK)
		{
			printf("Failed to initialize the system!\n");
			// Cleanup
			result = m_System->close();
			if (result != FMOD_OK)
			{
				printf("Failed to close system!\n");
			}
			return;
		}

		printf("AudioManager::Initialize(): Successful!\n");

		for (int i = 0; i < MAX_CHANNELS; i++)
		{
			m_ChannelList.push_back(new Channel);
			//m_ChannelList[0]->fmodChannel->set3DAttributes
		}

		// CLEANUP: Move creation of our channel groups into a separate function
		// Create our channel groups
		//FMOD::ChannelGroup* channelGroup;
		//result = m_System->createChannelGroup("Master", &channelGroup);
		//FMODCheckError(result);
		//m_ChannelGroupList.push_back(channelGroup);

		//result = m_System->createChannelGroup("SFX", &channelGroup);
		//FMODCheckError(result);
		//m_ChannelGroupList.push_back(channelGroup);

		//result = m_System->createChannelGroup("Music", &channelGroup);
		//FMODCheckError(result);
		//m_ChannelGroupList.push_back(channelGroup);

		//// Add our SFX and Music Channel Groups to our Master ChannelGroup
		////result = m_ChannelGroupList[CHANNELGROUP_MASTER_INDEX]->addGroup(m_ChannelGroupList[CHANNELGROUP_SFX_INDEX]);
		//result = GetMasterChannelGroup()->addGroup(GetSFXChannelGroup());
		//FMODCheckError(result);

		//result = GetMasterChannelGroup()->addGroup(GetMusicChannelGroup());
		//FMODCheckError(result);


		result = m_System->set3DSettings(.5f, .5f, 0.1f);
		result = m_System->setGeometrySettings(1000.0f);
		FMODCheckError(result);
		// Initialize()
		// Create our geometry
		// We will move this to another function once we have something working

		// Very basic example first
		result = m_System->createGeometry(2000, 8000, &m_Geometry);
		FMODCheckError(result);


		//// x [4.5, 5.5]
		//// z [-3, 3]
		//// y [-1, 3]

		//// bottom left, top left, bottom right	// clockwise
		//// top left, top right, bottom right	// clockwise

		////				  y+
		////		tl*-----------*tr
		////	  	  |\          |
		////		  |  \        |
		////		  |    \      |
		////	  z-  |      \    |	z+
		////		  |        \  |
		////		  |          \|
		////		bl*-----------*br
		////				y-
		//glm::vec3 bl(5.f, -1.f, -3.f);
		//glm::vec3 tl(5.f, 3.f, -3.f);
		//glm::vec3 br(5.f, -1.f, 3.f);
		//glm::vec3 tr(5.f, 3.f, 3.f);

		//const int numVertices = 6;
		//FMOD_VECTOR vertices[numVertices];
		//GLMToFMOD(bl, vertices[0]);
		//GLMToFMOD(tl, vertices[1]);
		//GLMToFMOD(br, vertices[2]);
		//GLMToFMOD(tl, vertices[3]);
		//GLMToFMOD(tr, vertices[4]);
		//GLMToFMOD(br, vertices[5]);

		//int polygonIndex = 0;
		//m_Geometry->addPolygon(1.f, 1.f, true, numVertices, vertices, &polygonIndex);
		//FMODCheckError(result);

		//bool isActive;
		//result = m_Geometry->getActive(&isActive);
		FMODCheckError(result);

		m_Initialized = true;
	}

	void AudioManager::Destroy()
	{
		if (!m_Initialized)
			return;

		FMOD_RESULT result;

		// Move this to a function, clear and release Audios from Audio map
		for (std::pair<const char*, Audio*> pair : m_AudioMap)
		{
			result = pair.second->Audio->release();
			FMODCheckError(result);
		}

		m_AudioMap.clear();

		result = m_System->close();
		FMODCheckError(result);

		result = m_System->release();
		FMODCheckError(result);

		m_Initialized = false;
	}

	//std::vector<DriverInfo> AudioManager::GetDeviceDrivers()
	//{
	//	int numDrivers;

	//	// Retrieve the number of audio drivers available on your PC
	//	m_System->getNumDrivers(&numDrivers);

	//	std::vector<DriverInfo> drivers;
	//	for (int i = 0; i < numDrivers; i++) {
	//		DriverInfo driverInfo;
	//		driverInfo.id = i;


	//		FMOD_RESULT result = m_System->getDriverInfo(
	//			driverInfo.id,
	//			driverInfo.name,
	//			256,
	//			&driverInfo.guid,
	//			&driverInfo.systemrate,
	//			&driverInfo.speakermode,
	//			&driverInfo.speakermodechannels
	//		);

	//		// Print to console
	//		printf("Device %d: \n name: %s\n guid: %d %d %d\n systemrate: \n speakermode: %d\n channels: %d\n", 
	//			driverInfo.id, 
	//			driverInfo.name, 
	//			driverInfo.guid.Data1, 
	//			driverInfo.guid.Data2, 
	//			driverInfo.guid.Data3, 
	//			driverInfo.systemrate, 
	//			(int)driverInfo.speakermode,
	//			driverInfo.speakermodechannels
	//		);
	//		drivers.push_back(driverInfo);
	//	}
	//	return drivers;
	//}

	//void AudioManager::SetDeviceDriver(int id)
	//{
	//	m_System->setDriver(id);
	//}

	// We can also load an audio file as a streaming asset
	// This will only load the audio resource data as needed
	// to play the audio file.
	void AudioManager::LoadAudioStream(const char* source)
	{
		// Ensure the Audio Manager is initialized before we do anything
		if (!m_Initialized)
			return;

		// Check out cache to see if the sound is already loaded.
		if (m_AudioMap.find(source) != m_AudioMap.end())
		{
			printf("AudioManager::LoadAudio() Audio already loaded!\n");
			return;
		}

		// Insert a key/value pair for the Audio with the source name as the key
		m_AudioMap.insert(std::pair<const char*, Audio*>(source, new Audio()));

		// Create a stream for the audio.
		// I changed FMOD_DEFAULT to FMOD_LOOP_NORMAL, this loops the audio
		FMOD_RESULT result = m_System->createStream(source, FMOD_DEFAULT, 0, &m_AudioMap[source]->Audio);
		if (result != FMOD_OK)
		{
			printf("AudioManager::LoadAudio() Failed to load the Audio stream: %s\n", source);
			return;
		}

		printf("AudioManager::LoadAudio(%s): Loaded successful!\n", source);
	}

	// This function loads a media file directly into memory
	void AudioManager::Load3DAudio(const char* file)
	{
		if (!m_Initialized)
			return;

		if (m_AudioMap.find(file) != m_AudioMap.end())
		{
			printf("AudioManager::LoadAudio() Audio already loaded!\n");
			return;
		}

		m_AudioMap.insert(std::pair<const char*, Audio*>(file, new Audio()));

		FMOD_RESULT result;

		// This call loads our audio file entirely into memory
		result = m_System->createSound(file, FMOD_3D | FMOD_LOOP_NORMAL, nullptr, &m_AudioMap[file]->Audio);
		FMODCheckError(result);
		if (result != FMOD_OK)
		{
			printf("AudioManager::LoadAudio() Failed to load the Audio file: %s\n", file);
			return;
		}

		m_AudioMap[file]->Audio->set3DMinMaxDistance(0.5f, 1000.f);
		printf("AudioManager::LoadAudio(%s): Loaded successful!\n", file);
	}

	// This function loads a media file directly into memory
	void AudioManager::LoadAudio(const char* file)
	{
		if (!m_Initialized)
			return;

		if (m_AudioMap.find(file) != m_AudioMap.end())
		{
			printf("AudioManager::LoadAudio() Audio already loaded!\n");
			return;
		}

		m_AudioMap.insert(std::pair<const char*, Audio*>(file, new Audio()));

		FMOD_RESULT result;

		// This call loads our audio file entirely into memory
		result = m_System->createSound(file, FMOD_DEFAULT, 0, &m_AudioMap[file]->Audio);
		if (result != FMOD_OK)
		{
			printf("AudioManager::LoadAudio() Failed to load the Audio file: %s\n", file);
			return;
		}

		printf("AudioManager::LoadAudio(%s): Loaded successful!\n", file);
	}

	int AudioManager::PlayAudio(const char* AudioName, glm::vec3 position)
	{
		// Ensure the Audio Manager is initialized before we do anything
		if (!m_Initialized)
		{
			printf("Not initialized!\n");
			return m_NextChannelId;	// Still want to return a valid channel id
		}

		// Check out cache to ensure the audio is loaded
		std::map<const char*, Audio*>::iterator it = m_AudioMap.find(AudioName);

		// If the audio isn't loaded, we can't play anything, so let the user know, and leave
		if (it == m_AudioMap.end())
		{
			printf("Audio not found!\n");
			return m_NextChannelId;	// Still want to return a valid channel id
		}
		printf("AudioFound!\n");

		// Get the current channel id, and calculate the next one
		int channelId = m_NextChannelId;
		m_NextChannelId = (m_NextChannelId + 1) % MAX_CHANNELS;	// Can replace with a helper function
		Channel* channel = m_ChannelList[channelId];

		// Play the sound and pass in a channel to reference
		FMOD_RESULT result = m_System->playSound(
			it->second->Audio,						// The sound to play
			nullptr, //m_ChannelGroupList[ChannelGroupIndex],	// ChannelGroup
			true,									// paused
			&channel->fmodChannel					// Channel returned
		);
		FMODCheckError(result);

		// We may not need this call since we call this often
		//SetListenerAttributes(
		//	glm::vec3(0),			// position
		//	glm::vec3(0),			// velocity
		//	glm::vec3(0, 0, 1),		// forward
		//	glm::vec3(0, 1, 0)		// up
		//);

		m_CurrentAudio = it->second;

		FMOD_VECTOR soundPos = { position.x, position.y, position.z };
		FMOD_VECTOR soundVel = { 0.f, 0.f, 0.f };
		result = channel->fmodChannel->set3DAttributes(&soundPos, &soundVel);
		FMODCheckError(result);

		result = channel->fmodChannel->setPaused(false);
		FMODCheckError(result);
		// Return the channel id used so we can modify it if needed.
		return channelId;
	}

	void AudioManager::PauseSpecificAudio(int ChannelId) {
		FMOD_RESULT result;
		//checks if the audio is currently paused or not, then pauses or unpauses it
		if (m_ChannelList[ChannelId]->playing) {
			result = m_ChannelList[ChannelId]->fmodChannel->setPaused(false);
			m_ChannelList[ChannelId]->playing = false;
		}
		else {
			result = m_ChannelList[ChannelId]->fmodChannel->setPaused(true);
			m_ChannelList[ChannelId]->playing = true;
		}
		FMODCheckError(result);
	}

	void AudioManager::StopAudio(int ChannelId) {
		FMOD_RESULT result;
		result = m_ChannelList[ChannelId]->fmodChannel->stop();
		FMODCheckError(result);
	}

	void AudioManager::Update()
	{
		if (!m_Initialized)
			return;

		FMOD_RESULT result;
		result = m_System->update();
		if (result != FMOD_OK)
		{
			FMODCheckError(result);
			Destroy();
		}
	}

















	/**										**/
	/**		ADD DSP EFFECTS TO CHANNEL		**/
	/**										**/


















/** REVERB DSP EFFECT **/

	void AudioManager::AddReverbFilterOnChannel(int channelId)
	{
		// value in the class header file
		// FMOD::DSP* m_ReverbDSP;

		FMOD_RESULT result = m_System->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &m_ReverbDSP);
		FMODCheckError(result);

		m_ChannelList[channelId]->fmodChannel->addDSP(0, m_ReverbDSP);
	}














	/** LOW PASS FILTER DSP EFFECT **/
	void AudioManager::AddLowPassFilterOnChannel(int channelId)
	{
		FMOD_RESULT result = m_System->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &m_LowPassDSP);
		FMODCheckError(result);

		result = m_LowPassDSP->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 5000);
		FMODCheckError(result);

		m_ChannelList[channelId]->fmodChannel->addDSP(1, m_LowPassDSP);
	}













	/** HIGH PASS FILTER DSP EFFECT **/

	void AudioManager::AddHighPassFilterOnChannel(int channelId)
	{
		FMOD_RESULT result = m_System->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &m_HighPassDSP);
		FMODCheckError(result);

		m_HighPassDSP->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, 500);
		FMODCheckError(result);

		m_ChannelList[channelId]->fmodChannel->addDSP(2, m_HighPassDSP);
	}







	/** DISTORTION PASS FILTER DSP EFFECT **/

	void AudioManager::AddDistortionFilterOnChannel(int channelId)
	{
		FMOD_RESULT result = m_System->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &m_DistortionDSP);
		FMODCheckError(result);

		m_DistortionDSP->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, 0.7f);
		FMODCheckError(result);

		m_ChannelList[channelId]->fmodChannel->addDSP(3, m_DistortionDSP);
	}







	/** CHROSU PASS FILTER DSP EFFECT **/

	void AudioManager::AddChorusPassOnChannel(int channelId)
	{
		FMOD_RESULT result = m_System->createDSPByType(FMOD_DSP_TYPE_CHORUS, &m_ChorusPassDSP);
		FMODCheckError(result);

		m_ChorusPassDSP->setParameterFloat(FMOD_DSP_CHORUS_MIX, 50.f);
		m_ChorusPassDSP->setParameterFloat(FMOD_DSP_CHORUS_RATE, 0.8f);
		m_ChorusPassDSP->setParameterFloat(FMOD_DSP_CHORUS_DEPTH, 3.f);
		FMODCheckError(result);

		m_ChannelList[channelId]->fmodChannel->addDSP(4, m_ChorusPassDSP);
	}








	/**										**/
	/**		MODIFY DSP EFFECTS ON CHANNEL	**/
	/**										**/








/** MODIFYING REVERB DSP EFFECT **/
	void AudioManager::SetReverbValuesOnChannel(int channelId, float decay, float density, float diffusion)
	{
		FMOD_RESULT result;

		// Concert Hall, settings, decay 3900ms, density, diffusion set to 100

		result = m_ReverbDSP->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, decay);
		result = m_ReverbDSP->setParameterFloat(FMOD_DSP_SFXREVERB_DENSITY, density);
		result = m_ReverbDSP->setParameterFloat(FMOD_DSP_SFXREVERB_DIFFUSION, diffusion);

		FMODCheckError(result);
	}










	/** MODIFYING LOW PASS FILTER DSP EFFECT **/
	void AudioManager::SetLowPassFilterValuesOnChannel(int channelId, float cutoff)
	{
		FMOD_RESULT result = m_LowPassDSP->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, cutoff);
		FMODCheckError(result);
	}














	/** MODIFYING HIGH PASS FILTER DSP EFFECT **/
	void AudioManager::SetHighPassFilterValuesOnChannel(int channelId, float cutoff)
	{
		FMOD_RESULT result = m_HighPassDSP->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, cutoff);
		FMODCheckError(result);
	}














	/** MODIFYING DISTORTION PASS FILTER DSP EFFECT **/

	void AudioManager::SetDistortionLevelFilterValuesOnChannel(int channelId, float level)
	{
		FMOD_RESULT result = m_DistortionDSP->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, level);
		FMODCheckError(result);
	}














	/** MODIFYING CHROUS PASS FILTER DSP EFFECT **/

	void AudioManager::SetChorusPassValuesOnChannel(int channelId, float mix, float rate, float depth)
	{
		FMOD_RESULT result;
		result = m_ChorusPassDSP->setParameterFloat(FMOD_DSP_CHORUS_MIX, mix);
		result = m_ChorusPassDSP->setParameterFloat(FMOD_DSP_CHORUS_RATE, rate);
		result = m_ChorusPassDSP->setParameterFloat(FMOD_DSP_CHORUS_DEPTH, depth);
		FMODCheckError(result);
	}












	//	SET CHANNEL API CALLS

	void AudioManager::SetChannelVolume(int id, float value)
	{
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setVolume(value);
		FMODCheckError(result);
	}

	void AudioManager::SetChannelPitch(int id, float value)
	{
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setPitch(value);
		FMODCheckError(result);
	}

	void AudioManager::SetChannelPan(int id, float value)
	{
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setPan(value);
		FMODCheckError(result);
	}



	// SET CHANNEL GROUP API CALLS

	void AudioManager::SetChannelGroupVolume(int id, float value)
	{
		printf("Set ChannelGroup Volume %d to %.2f\n", id, value);
		FMOD_RESULT result = m_ChannelGroupList[id]->setVolume(value);
		FMODCheckError(result);
	}



	void AudioManager::SetChannelGroupPitch(int id, float value)
	{
		FMOD_RESULT result = m_ChannelGroupList[id]->setPitch(value);
		FMODCheckError(result);
	}



	void AudioManager::SetChannelGroupPan(int id, float value)
	{
		FMOD_RESULT result = m_ChannelGroupList[id]->setPan(value);
		FMODCheckError(result);
	}






	bool AudioManager::GetChannelPlaying(int id)
	{
		if (id == -1) {
			return false;
		}
		bool isPlaying;
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->isPlaying(&isPlaying);
		//FMODCheckError(result);
		return isPlaying;
	}

	void AudioManager::GetPlaybackPosition(int id, unsigned int& value)
	{
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->getPosition(&value, FMOD_TIMEUNIT_MS);
		FMODCheckError(result);
	}

	void AudioManager::SetListenerAttributes(const glm::vec3& position,
		const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up)
	{
		FMOD_VECTOR fmodPosition;
		FMOD_VECTOR fmodVelocity;
		FMOD_VECTOR fmodForward;
		FMOD_VECTOR fmodUp;

		GLMToFMOD(position, fmodPosition);
		GLMToFMOD(velocity, fmodVelocity);
		GLMToFMOD(forward, fmodForward);
		GLMToFMOD(up, fmodUp);

		FMOD_RESULT result = m_System->set3DListenerAttributes(
			0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp
		);

		float direct, reverb;
		FMOD_VECTOR origin;
		GLMToFMOD(glm::vec3(0.f), origin);
		m_System->getGeometryOcclusion(&origin, &fmodPosition, &direct, &reverb);
		//printf("ListenerPosition: %.2f, %.2f, %.2f | direct: %.4f, reverb: %.4f\n", position.x, position.y, position.z, direct, reverb);
		//FMODCheckError(result);
	}

	void AudioManager::GetListenerAttributes(glm::vec3& position,
		glm::vec3& velocity, glm::vec3& forward, glm::vec3& up)
	{
		FMOD_VECTOR fmodPosition;
		FMOD_VECTOR fmodVelocity;
		FMOD_VECTOR fmodForward;
		FMOD_VECTOR fmodUp;

		FMOD_RESULT result = m_System->get3DListenerAttributes(
			0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp
		);
		FMODCheckError(result);

		FMODToGLM(fmodPosition, position);
		FMODToGLM(fmodVelocity, velocity);
		FMODToGLM(fmodForward, forward);
		FMODToGLM(fmodUp, up);
	}


	int AudioManager::AddPolygon(float direct, float reverb, bool doublesided,
		const std::vector<glm::vec3>& vertices, const glm::vec3& position)
	{
		FMOD_RESULT result;
		int index;

		// Add the polygon
		int numVertices = vertices.size();
		FMOD_VECTOR* fmodVertices = (FMOD_VECTOR*)malloc(sizeof(FMOD_VECTOR) * numVertices);
		for (int i = 0; i < numVertices; i++) {
			GLMToFMOD(vertices[i], fmodVertices[i]);
			//printf("%.2f, %.2f, %.2f\n", fmodVertices[i].x, fmodVertices[i].y, fmodVertices[i].z);
		}

		result = m_Geometry->addPolygon(direct, reverb, doublesided, numVertices, fmodVertices, &index);
		FMODCheckError(result);

		// Set the position
		FMOD_VECTOR fmodPosition;
		GLMToFMOD(position, fmodPosition);
		result = m_Geometry->setPosition(&fmodPosition);
		FMODCheckError(result);

		glm::vec3 scale(1.f);
		FMOD_VECTOR fmodScale;
		GLMToFMOD(scale, fmodScale);
		result = m_Geometry->setScale(&fmodScale);
		FMODCheckError(result);

		FMOD_VECTOR v;
		glm::vec3 g;
		for (int i = 0; i < numVertices; i++)
		{
			m_Geometry->getPolygonVertex(index, i, &v);
			FMODToGLM(v, g);
			printf("%.2f, %.2f, %.2f = %.2f, %.2f, %.2f\n", g.x, g.y, g.z,
				vertices[i].x, vertices[i].y, vertices[i].z);
		}

		m_Geometry->setActive(true);

		//int count;
		//m_Geometry->setPolygonAttributes(index, 0.9f, 0.9f, true);

		return index;
	}

	unsigned int AudioManager::GetData(const char* sound/*, void* data*/)
	{
		Audio* audio = m_AudioMap[sound];
		unsigned int length = 0;
		audio->Audio->getLength(&length, FMOD_TIMEUNIT_RAWBYTES);
		unsigned int bytesRead;
		//FMOD_RESULT result = audio->Audio->readData(data, length, &bytesRead);
		//FMODCheckError(result);
		return length;
	}

	void AudioManager::PrintInfo() const
	{
	}

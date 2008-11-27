#include <lv/DSoundAudio/DSMusicBuffer.hpp>
#include <lv/DSoundAudio/DSoundHelper.hpp>
#include <lv/DSoundAudio/DSoundEngine.hpp>

namespace lv
{
	DSMusicBuffer::DSMusicBuffer(AudioDataSourcePtr data)
		: data_(data)
	{
		/*
		WAVEFORMATEX wf;
		DSoundHealper::fill_format(wf, *data);

		IDirectSound8 & dsound = static_cast<DSoundEngine&>(AudioEngine::instance()).dsound();

		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(dsbd));
		dsbd.dwSize = sizeof(dsbd);
		dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME;
		dsbd.dwBufferBytes = data.size();
		dsbd.lpwfxFormat = &wf;

		LPDIRECTSOUNDBUFFER buf;
		IDirectSound8 & dsound = static_cast<DSoundEngine&>(AudioEngine::instance()).dsound();
		DX_TIF(dsound.CreateSoundBuffer(&dsbd, &buf, NULL));

		buf_num = std::max(buf_num, 1U);
		buffers_.resize(buf_num);
		buffers_[0] = com_ptr(buf);

		for(size_t i = 1; i < buf_num; ++i)
		{
			LPDIRECTSOUNDBUFFER temp_buf;
			DX_TIF(dsound.DuplicateSoundBuffer(buf, &temp_buf));
			buffers_[i] = com_ptr(temp_buf);
		}

		// fill the buffer
		char * locked_ptr;
		DWORD locked_size;
		DX_TIF(buf->Lock(0, data.size(), reinterpret_cast<void**>(&locked_ptr), &locked_size, 
			NULL, NULL, DSBLOCK_FROMWRITECURSOR));
		size_t read_size = std::min(data.size(), static_cast<size_t>(locked_size));
		read_size = data.read(BufferRef(locked_ptr, read_size));

		BOOST_ASSERT(read_size == data.size());

		buf->Unlock(locked_ptr, locked_size, NULL, 0);

		// reset
		data.reset();
		*/
	}
}
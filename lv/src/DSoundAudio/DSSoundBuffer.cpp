#include <lv/DSoundAudio/DSSoundBuffer.hpp>
#include <lv/DSoundAudio/DSoundEngine.hpp>
#include <lv/DSoundAudio/DSoundHelper.hpp>
#include <lv/SharedPtr.hpp>
#include <lv/Foreach.hpp>
#include <lv/Exception.hpp>
#include <boost/functional.hpp>
#include <xutility>

namespace lv
{

	bool free_buffer(boost::shared_ptr<IDirectSoundBuffer> const & buf)
	{
		DWORD status;
		DX_VERIFY(buf->GetStatus(&status));
		return !(status & DSBSTATUS_PLAYING);
	}

	DSSoundBuffer::DSSoundBuffer(AudioDataSource & data, size_t buf_num)
	{
		WAVEFORMATEX wf;
		DSoundHealper::fill_format(wf, data);

		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(dsbd));
		dsbd.dwSize = sizeof(dsbd);
		dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
		dsbd.dwBufferBytes = data.size();
		dsbd.guid3DAlgorithm = GUID_NULL;
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
	}


	void DSSoundBuffer::play(bool loop /* = false */)
	{
		buffer_vec::iterator it = std::find_if(buffers_.begin(), buffers_.end(), free_buffer);
		if(it == buffers_.end())
			it = buffers_.begin();

		LPDIRECTSOUND3DBUFFER ds3dbuf;
		DX_VERIFY((*it)->QueryInterface(IID_IDirectSound3DBuffer, reinterpret_cast<void**>(&ds3dbuf)));
		if(ds3dbuf != NULL)
		{
			DX_VERIFY(ds3dbuf->SetPosition(pos_.x, pos_.y, pos_.z, DS3D_IMMEDIATE));
			ds3dbuf->Release();
		}

		DX_VERIFY((*it)->Play(0, 0, loop ? DSBPLAY_LOOPING : 0));
	}


	void DSSoundBuffer::stop()
	{
		foreach(dsbuffer_ptr & buf, buffers_)
		{
			DX_VERIFY(buf->Stop());
			DX_VERIFY(buf->SetCurrentPosition(0));
		}
	}

	bool DSSoundBuffer::playing() const
	{
		return std::find_if(buffers_.begin(), buffers_.end(), boost::not1(free_buffer)) == buffers_.end();
	}

	void DSSoundBuffer::set_volume(float vol)
	{
		foreach(dsbuffer_ptr & buf, buffers_)
		{
			DX_VERIFY(buf->SetVolume(DSoundHealper::linear_to_db(vol)));
		}
	}

	void DSSoundBuffer::reset()
	{
		stop();
	}

}
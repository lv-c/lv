#include <lv/Audio/WavSource.hpp>
#include <lv/BinaryStream/BinaryIStream.hpp>
#include <lv/BinaryStream/Manipulators.hpp>
#include <lv/FourCC.hpp>

#include <boost/assert.hpp>

namespace lv
{
	WavSource::WavSource(IStreamPtr is)
		: istream_(is)
		, data_offset_(0)
	{
		static uint32 const riff_id = FourCC<'R', 'I', 'F', 'F'>::value;
		static uint32 const wave_id = FourCC<'W', 'A', 'V', 'E'>::value;
		static uint32 const fmt_id = FourCC<'f', 'm', 't', ' '>::value;
		static uint32 const data_id = FourCC<'d', 'a', 't', 'a'>::value;

		using namespace bstream;

		BinaryIStream bin_is(*is);

		try
		{
			bin_is >> check_equal(riff_id) >> forward(4) >> check_equal(wave_id);


			while (true)
			{
				uint32	chunk_id, chunk_size;

				try
				{
					bin_is >> chunk_id >> chunk_size;
				}
				catch (std::ios_base::failure const &)
				{
					if (data_offset_ == 0)
					{
						throw InvalidAudioData();
					}

					bin_is.seekg(data_offset_);

					return;
				}

				switch (chunk_id)
				{
				case fmt_id:
					{
						uint32 fmt_chunk_size, freq;
						uint16	channels, bits_per_sample;
						bin_is >> check_equal(fmt_id) >> fmt_chunk_size >> check_equal(uint16(1), UnSupportedAudioFormat())
							>> channels >> freq >> forward(6) >> bits_per_sample;

						this->channels_ = channels;
						this->bits_per_sample_ = bits_per_sample;

						this->freq_ = freq;

						bin_is >> forward(fmt_chunk_size - 16);
					}
					break;

				case data_id:
					data_offset_ = bin_is.tellg();
					size_ = chunk_size;
					bin_is >> forward(chunk_size);
					break;

				default:
					bin_is >> forward(chunk_size);
					break;
				}
			}
		}
		catch (CheckEqualError const &)
		{
			throw InvalidAudioData();
		}
	}


	size_t	WavSource::read(BufferRef buf)
	{
		try
		{
			istream_->read(buf.data(), buf.size());
		}
		catch (std::ios_base::failure const &)
		{
			BOOST_ASSERT(false);
		}

		return static_cast<size_t>(istream_->gcount());
	}

	void	WavSource::reset()
	{
		istream_->seekg(data_offset_);
	}
}

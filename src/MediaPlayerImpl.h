#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "MediaMessageQueue.h"

#include "SDL.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
}
#endif

// 播放倍速
typedef enum _MEDIA_PLAYER_SPEED
{
	MEDIA_PLAYER_SPEED_NONE = 0,  // 不支持倍速
	MEDIA_PLAYER_SPEED_QUARTER,   // 0.25倍速
	MEDIA_PLAYER_SPEED_HALF,      // 0.5倍速
	MEDIA_PLAYER_SPEED_NORMAL,    // 1倍速
	MEDIA_PLAYER_SPEED_DOUBLE,    // 2倍速
	MEDIA_PLAYER_SPEED_QUADRUPLE, // 4倍速
	MEDIA_PLAYER_SPEED_MAX        // 封顶
} MEDIA_PLAYER_SPEED;

typedef struct _audio_info_t
{
    int volume;
    int len;
	int index;        // 播放位置
    uint8_t * chunk;
    uint8_t * pos;
} audio_info_t;

class CMediaPlayerRescaler;

class CMediaPlayerImpl
{
public:
	CMediaPlayerImpl() = default;
	~CMediaPlayerImpl() = default;

public:
	// 打开文件
	bool open(const char* url);

	// 关闭文件
	void close();

	// 获取文件时长
	int64_t getVideoDuration();

	// 获取当前播放位置
	int64_t getVideoPos();

	// 开始播放
	bool start(const void* wnd, int width, int height);

	// 暂停
	bool pause();

	// 停止
	bool stop();

	// 快进
	bool forward();

	// 快退
	bool backward();

protected:
	// 创建解码器
	AVCodecContext * createDecoder(int stream_index);

	// 销毁解码器
	void destoryDecoder(AVCodecContext ** decoder);

	// 视频帧解码
	bool decodeVideoPacket(const AVPacket * pkt, AVFrame * frm, bool * got);

	// 音频帧解码
	bool decodeAudioPacket(const AVPacket * pkt, AVFrame * frm, bool * got);

	// 创建视频播放器
	bool createVideoPlayer(const void * wnd, int width, int height);

	// 销毁视频播放器
	void destoryVideoPlayer();

	// 创建音频播放器
	bool createAudioPlayer();

	// 销毁音频播放器
	void destoryAudioPlayer();

	// 创建音频转换器
	bool createAudioRescaler();

	//销毁音频转换器
	void destoryAudioRescaler();

	// 接收数据包线程
	void recvPacketsThr();

	// 处理视频数据线程
	void dealVideoPacketsThr();

	// 处理音频数据线程
	void dealAudioPacketsThr();

	// 获取音频时钟
	double getAudioClock();

private:
	// 是否初始化
	std::atomic_bool _is_init = { false };

	// 是否文件结束
	std::atomic_bool _is_over = { false };
	// 是否播放中
	std::atomic_bool _is_playing = { false };
	// 是否暂停中
	std::atomic_bool _is_pause = { false };

	// 数据包线程锁
	std::mutex _packets_mtx;
	// 读取数据包线程
	std::thread _packets_thr;
	// 读取数据包变量
	std::condition_variable _packets_cond;
	
	// 视频数据线程锁
	std::mutex _video_mtx;
	// 处理视频数据线程
	std::thread _video_thr;
	// 视频线程变量
	std::condition_variable _video_cond;

	// 音频数据线程锁
	std::mutex _audio_mtx;
	// 处理音频数据线程
	std::thread _audio_thr;
	// 音频解码线程变量
	std::condition_variable _audio_cond;

	// 视频流索引
	std::atomic_int _video_index = { -1 };
	// 视频解码器
	AVCodecContext * _video_decoder = nullptr;

	// 当前使用音频流索引
	std::atomic_int _audio_cur_index = { -1 };
	// 视频流索引数组
    std::vector<int> _audio_index;
	// 音频解码器
	AVCodecContext * _audio_decoder = nullptr;
	// 音频时钟
	double _audio_clock = 0.0;

	// 视频像素格式
	AVPixelFormat _pix_fmt = AVPixelFormat::AV_PIX_FMT_NONE;
	// 输入流上下文
	AVFormatContext * _fmt_ctx = nullptr;
	// 视频流
	AVStream * _video_stream = nullptr;
	// 音频流
	AVStream * _audio_stream = nullptr;

	// 屏幕宽度
	int _wnd_width = 0;
	// 屏幕高度
	int _wnd_height = 0;
	// 视频帧宽度
	int _frm_width = 0;
	// 视频帧高度
	int _frm_height = 0;

	audio_info_t * _audio_info = nullptr;
	// 视频转换器
	CMediaPlayerRescaler * _rescaler = nullptr;
	// 音频转换器
	SwrContext * _audio_rescaler = nullptr;

	// 位置
	SDL_Rect _sdl_rect = { 0 };
	// 播放器句柄
	SDL_Window * _player_wnd = nullptr;
	// 渲染器
	SDL_Renderer * _sdl_render = nullptr;
	// 色泽纹理
	SDL_Texture * _sdl_texture = nullptr;

	// 视频包队列
	CMediaMessageQueue<AVPacket> _video_queue;
	// 音频包队列
	CMediaMessageQueue<AVPacket> _audio_queue;

	// 音频音量
	std::atomic_int _volume = { 64 };
	// 播放倍速 默认正常倍速
	MEDIA_PLAYER_SPEED _speed = MEDIA_PLAYER_SPEED_NORMAL;
};


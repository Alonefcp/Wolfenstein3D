#include "RenderThread.h"
#include "..\Others\LatencyChecker.h"

ConcurrentQueue<RenderCommand> RenderThread::_commandQ;
std::thread* RenderThread::_thread = nullptr;
atomic<bool> RenderThread::_run(false);

void RenderThread::init()
{
	_run.store(true);
	_thread = new std::thread(&RenderThread::loop);
}

void RenderThread::release()
{
	_run.store(false);
	present();
	_thread->join();
	_commandQ.releaseQueue();
	delete _thread;
	_thread = nullptr;
}

// if thread is running, one render command is popped from the queue and executed
void RenderThread::loop()
{
	//Thread affinity for PS4
#ifdef PLATFORM_PS4
	scePthreadSetaffinity(scePthreadSelf(), 0x10);
#endif
	while (_run.load()) {
		RenderCommand rc;
		_commandQ.pop(rc);

		switch (rc.rcType)
		{
		case RenderCommandType::DRAW_RECT:
			Renderer::drawRect(rc.iDrawRect.colorUp, rc.iDrawRect.colorDown);
			break;
		case RenderCommandType::CLEAR:
			Renderer::clear(rc.iClear.color);
			break;
		case RenderCommandType::DRAW_VERTICAL_LINE:
			Renderer::drawVerticalLine(rc.iDrawVerticalLine.x, rc.iDrawVerticalLine.y1, rc.iDrawVerticalLine.y2, rc.iDrawVerticalLine.color);
			break;
		case RenderCommandType::DRAW_VERTICAL_TEXTURED_LINE:
			Renderer::drawVerticalTexturedLine(rc.iDrawVerticalTexturedLine.x, rc.iDrawVerticalTexturedLine.y,
				rc.iDrawVerticalTexturedLine.texX, rc.iDrawVerticalTexturedLine.height, rc.iDrawVerticalTexturedLine.img);
			break;
		case RenderCommandType::DRAW_IMAGE:
			Renderer::drawImage(rc.iDrawImage.x1, rc.iDrawImage.x2, rc.iDrawImage.y1, rc.iDrawImage.y2, rc.iDrawImage.im);
			break;
		case RenderCommandType::DRAW_LINE:
			Renderer::drawLine(rc.iDrawLine.x1, rc.iDrawLine.x2, rc.iDrawLine.y1, rc.iDrawLine.y2, rc.iDrawLine.color);
			break;
		case RenderCommandType::PRESENT:
			Renderer::present();
			LatencyChecker::substractLatency();
			break;
		default:
			break;
		}
	}
}

void RenderThread::enqueueCommand(RenderCommand rc)
{
	_commandQ.push(rc);
}

bool RenderThread::emptyQueue()
{
	return _commandQ.empty();
}

void RenderThread::drawRect(uint32_t colorUp, uint32_t colorDown)
{
	RenderCommand rc = { RenderCommandType::DRAW_RECT };
	rc.iDrawRect.colorUp = colorUp;
	rc.iDrawRect.colorDown = colorDown;
	enqueueCommand(rc);
}

void RenderThread::clear(uint32_t color)
{
	RenderCommand rc = { RenderCommandType::CLEAR };
	rc.iClear.color = color;
	enqueueCommand(rc);
}

void RenderThread::drawVerticalTexturedLine(int x, int y, int texX, int height, Image* img)
{
	RenderCommand rc = { RenderCommandType::DRAW_VERTICAL_TEXTURED_LINE };
	rc.iDrawVerticalTexturedLine.x = x;
	rc.iDrawVerticalTexturedLine.y = y;
	rc.iDrawVerticalTexturedLine.texX = texX;
	rc.iDrawVerticalTexturedLine.height = height;
	rc.iDrawVerticalTexturedLine.img = img;
	enqueueCommand(rc);
}

void RenderThread::drawVerticalLine(int x, int y1, int y2, uint32_t color)
{
	RenderCommand rc = { RenderCommandType::DRAW_VERTICAL_LINE };
	rc.iDrawVerticalLine.x = x;
	rc.iDrawVerticalLine.y1 = y1;
	rc.iDrawVerticalLine.y2 = y2;
	rc.iDrawVerticalLine.color = color;
	enqueueCommand(rc);
}

void RenderThread::drawImage(int x1, int x2, int y1, int y2, Image* im)
{
	RenderCommand rc = { RenderCommandType::DRAW_IMAGE };
	rc.iDrawImage.x1 = x1;
	rc.iDrawImage.x2 = x2;
	rc.iDrawImage.y1 = y1;
	rc.iDrawImage.y2 = y2;
	rc.iDrawImage.im = im;
	enqueueCommand(rc);
}

void RenderThread::drawLine(int x1, int x2, int y1, int y2, uint32_t color)
{
	RenderCommand rc = { RenderCommandType::DRAW_LINE };
	rc.iDrawLine.x1 = x1;
	rc.iDrawLine.x2 = x2;
	rc.iDrawLine.y1 = y1;
	rc.iDrawLine.y2 = y2;
	rc.iDrawLine.color = color;
	enqueueCommand(rc);
}

void RenderThread::present()
{
	RenderCommand rc = { RenderCommandType::PRESENT };
	enqueueCommand(rc);
	LatencyChecker::addLatency();
}

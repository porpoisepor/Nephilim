#include <Nephilim/StateTransitionBlocks.h>
#include <Nephilim/Logger.h>
#include <Nephilim/NxMath.h>
#include <Nephilim/Time.h>
#include <Nephilim/Renderer.h>


NEPHILIM_NS_BEGIN

StateTransitionBlocks::StateTransitionBlocks()
{
	Log("Transition start.");

	blockSize = 150.f;

	remainingTime = 0.f;
	m_next = false;

	for(int i = 0; i < 1024 / blockSize; ++i)
	{
		for(int j = 0; j < 768 / blockSize; ++j)
		{
			vec2 p(i * blockSize, j * blockSize);
			positions.push_back(p);
		}
	}

	for(int i = 0; i < 300; ++i)
	{
		int p1 = Math::randomInt(0,positions.size() - 1);
		int p2 = Math::randomInt(0,positions.size() - 1);

		vec2 temp = positions[p2];
		positions[p2] = positions[p1];
		positions[p1] = temp;
	}
}

void StateTransitionBlocks::onUpdate(const Time& time)
{
	if(!m_next)
	{
		remainingTime -= time.asSeconds();

		if(remainingTime <= 0.f && !positions.empty())
		{
			vec2 p = positions.back();
			positions.pop_back();

			RectangleShape shape;
			shape.setColor(Color::Black);
			shape.setSize(blockSize,blockSize);
			shape.setPosition(p.x, p.y);
			shapes.push_back(shape);

			remainingTime = 0.005f;
		}

		if(positions.empty())
			m_next = true;
	}
	else
	{
		remainingTime -= time.asSeconds();

		if(remainingTime <= 0.f)
		{
			if(shapes.size() > 0) shapes.pop_back();

			remainingTime = 0.005f;
		}

		if(shapes.empty())
		{
			Log("Transition ended");
			finish();
		}
	}

}

void StateTransitionBlocks::draw(Renderer* renderer)
{
	if(!m_next)
		drawPreviousFrame(renderer);
	else
		drawNextFrame(renderer);

	for(size_t i = 0; i < shapes.size(); ++i)
	{
		renderer->draw(shapes[i]);
	}
}

NEPHILIM_NS_END
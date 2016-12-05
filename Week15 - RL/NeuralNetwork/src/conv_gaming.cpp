/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////


#include "BreakoutGame.h"
#include "NeuralNetwork.h"
#include "CircularQueue.h"

#pragma comment(lib, "winmm.lib")

BreakoutGame game_;
NeuralNetwork nn_;
CircularQueue<VectorND<float> > history_;

const float dt = 0.04f;
bool is_training = true;

void initializeAI();
void playAI();
void playHuman();
void render();

void main()
{
	//std::cout << Convolution2D::getDesiredOutputRes(5, 3, 2, 1) << std::endl;
	//return;
	
//  playHuman();

    initializeAI();
    playAI();
}

void initializeAI()
{
    const int num_histories = 4;
	const int num_channels = 5;

    nn_.initialize(game_.getNumStateVariables() * num_histories, 3, 3);

    nn_.layers_[0].act_type_ = LayerBase::LReLU;
	//conn 0 : filter 5x5
	nn_.layers_[1].act_type_ = LayerBase::LReLU;
	//conn 1 : filter 3x3
	nn_.layers_[2].act_type_ = LayerBase::LReLU;
	//conn 2 : averaging
	nn_.layers_[3].act_type_ = LayerBase::LReLU;
	//conn 3 : full
	nn_.layers_[4].act_type_ = LayerBase::LReLU;

    nn_.eta_ = 1e-3;
    nn_.alpha_ = 0.9;

	nn_.layers_[0].initialize(num_histories * SCR_WIDTH * SCR_HEIGHT * num_channels + 1, LayerBase::LReLU);

	{
		ConvFilter2D filter;
		filter.initialize(5, 5, 4, 4, 1, 1, 0.1, 0.01);
		ConvImage2D im;
		im.width_ = SCR_WIDTH;
		im.height_ = SCR_HEIGHT;
		ConvImage2D om;
		om.width_ = 5;
		om.height_ = 5;

		ConvConnection2D *new_conv = nn_.setConvConnection2D(0);

		int in_count = 0;
		int out_count = 0;
		for (int h = 0; h < num_histories; h++)
		{
			for (int ch = 0; ch < num_channels; ch++)
			{
				new_conv->channel_list_.push_back(new ConvChannel2D(filter, (T)0.5, (T)0.3, im, in_count, om, out_count));

				out_count += om.width_*om.height_;
			}

			in_count += im.width_ * im.height_; // one image to multi channels
		}
	}

	nn_.layers_[1].initialize(num_histories * 5 * 5 * num_channels + 1, LayerBase::LReLU);

	{
		ConvFilter2D filter;
		filter.initialize(3, 3, 2, 2, 1, 1, 0.1, 0.01);
		ConvImage2D im;
		im.width_ = 5;
		im.height_ = 5;
		ConvImage2D om;
		om.width_ = 3;
		om.height_ = 3;

		int in_count = 0;
		int out_count = 0;
		
		ConvConnection2D *new_conv = nn_.setConvConnection2D(1);

		for (int h = 0; h < num_histories; h++)
		{
			for (int ch = 0; ch < num_channels; ch++)
			{
				new_conv->channel_list_.push_back(new ConvChannel2D(filter, (T)0.5, (T)0.3, im, in_count, om, out_count));
			
				in_count += im.width_ * im.height_;
				out_count += om.width_ * om.height_; // one channel to one channel
			}
		}
	}

	nn_.layers_[2].initialize(num_histories * 3 * 3 * num_channels + 1, LayerBase::LReLU);

	{
		ConvFilter2D filter;
		filter.initialize(3, 3, 1, 1, 0, 0, 0.1, 0.01);
		ConvImage2D im;
		im.width_ = 3;
		im.height_ = 3;
		ConvImage2D om;
		om.width_ = 1;
		om.height_ = 1;

		int in_count = 0;
		int out_count = 0;

		ConvConnection2D *new_conv = nn_.setConvConnection2D(2);

		for (int h = 0; h < num_histories; h++)
		{
			for (int ch = 0; ch < num_channels; ch++)
			{
				new_conv->channel_list_.push_back(new ConvChannel2D(filter, (T)0.5, (T)0.3, im, in_count, om, out_count));

				in_count += im.width_ * im.height_;
				out_count += om.width_ * om.height_; // one channel to one channel
			}
		}
	}

	nn_.layers_[3].initialize(num_histories * num_channels + 1, LayerBase::LReLU);
	nn_.setFullConnection(3, 0.5f, 0.3f);

	render(); // need to render for conv

    history_.initialize(num_histories);

    for (int h = 0; h < history_.array_.num_elements_; h++)
        history_.pushBack(game_.getStateBuffer());
}

void render()
{
    const int ball_i = round(game_.ball_x_ * (float)SCR_WIDTH - 1);
    const int ball_j = round(game_.ball_y_ * (float)SCR_HEIGHT - 1);

    int paddle_i = round(game_.paddle_x_ * (float)SCR_WIDTH - 1);

    if (paddle_i < 0) paddle_i = 0;
    if (paddle_i > SCR_WIDTH - 1 - 6) paddle_i = SCR_WIDTH - 1 - 6;

    game_.renderer_.drawToBackBuffer(paddle_i, 0, "------");
    game_.renderer_.drawToBackBuffer(ball_i, ball_j, "*");

	if(is_training == false)
		game_.renderer_.render();
	else
	{
		for (int j = 0; j < SCR_HEIGHT; j++)
			for (int i = 0; i < SCR_WIDTH; i++)
			{
				game_.renderer_.front_buffer[j][i] = game_.renderer_.back_buffer[j][i];
				game_.renderer_.back_buffer[j][i] = '\0';
			}
	}

    Sleep(20);
}

void playAI()
{
    VectorND<T> input, next_input;
    input.initialize(nn_.num_input_, true);
    next_input.initialize(nn_.num_input_, true);

    const float gamma = 0.95;

	//long start_time = timeGetTime();
    while (1)   // game main loop
	//for(int rep = 0; rep < 1000; rep ++)
    {
        static bool is_pushed = false;

		if (GetConsoleWindow() == GetForegroundWindow() && GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            if (is_pushed == false)
            {
                is_training = !is_training;

                system("cls");
                game_.renderer_.resetBuffers();

				if (is_training == true)
				{
					std::cout << "Background Training " << nn_.eta_ << std::endl;
				}

                is_pushed = true;
            }
        }
        else
        {
            is_pushed = false;
        }

        int count = 0;
        for (int h = 0; h < history_.array_.num_elements_; h++)
        {
            const VectorND<float> &state = history_.getValue(h);

            input.copyPartial(state, count, 0, game_.getNumStateVariables());

            count += game_.getNumStateVariables();
        }

        nn_.setInputVector(input);
        nn_.feedForward();

        const int selected_dir = is_training == true ? nn_.getOutputIXEpsilonGreedy(0.1) : nn_.getOutputIXEpsilonGreedy(0.0);

        switch (selected_dir)
        {
        case 2:
            game_.movePad(-0.33*game_.paddle_width_);
            break;
        case 1:
            game_.movePad(0.33*game_.paddle_width_);
            break;
        case 0:
            // do nothing
            break;
        default:
            std::cout << "Wrong direction " << std::endl;
        }

        float reward = 0.0f;

        game_.updateBall(dt, reward);

		render(); // need to render for conv

        static int counter = 0;
        static int counter_all = 0;

        if (reward > 0.5f) counter++;
        if (reward != 0.0f) counter_all++;

        if (counter_all == 1000)
        {
            static double time = timeGetTime();

            if(is_training == true)
                std::cout << (float)counter / (float)counter_all *100.0f << " % " << (timeGetTime() - time) / 1000.0f << " sec"<<std::endl;

            time = timeGetTime();

            counter = 0;
            counter_all = 0;
        }

        if (reward < 0.0f) reward = 0.0f;

        VectorND<T> reward_vector;
        nn_.copyOutputVectorTo(false, reward_vector);

		//game_.printStateBuffer();
		//exit(1);

        // update state
        history_.pushBack(game_.getStateBuffer());

        // make next input
        for (int h = 0, count = 0; h < history_.array_.num_elements_; h++, count += game_.getNumStateVariables())
        {
            const VectorND<float> &state = history_.getValue(h);
            next_input.copyPartial(state, count, 0, game_.getNumStateVariables());
        }

        nn_.setInputVector(next_input);
        nn_.feedForward();

        const float next_Q = nn_.getOutputValueMaxComponent();

		reward_vector[selected_dir] = reward + gamma * next_Q;

		if (is_training == true)
		{
			nn_.setInputVector(input);
			nn_.feedForward();
			nn_.propBackward(reward_vector);
		}

		if (is_training == false)
        {


            if (reward > 0.5f) printf("\a");
        }

        //render();
    }
	//long end_time = timeGetTime();

	//std::cout << "elapsed time = " << end_time - start_time << std::endl;
}

void playHuman()
{
    while (1)   // game main loop
    {
        // Use GetAsyncKeyState for multiple key inputs
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        {
            game_.movePad(-0.1 / (float)SCR_WIDTH);
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        {
            game_.movePad(0.1 / (float)SCR_WIDTH);
        }

        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
        }
        else if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
        }

        float reward = 0.0f;

        game_.updateBall(0.005f, reward);

        if (reward > 0.0) printf("\a");

        const int ball_i = game_.ball_x_ * (float)SCR_WIDTH;
        const int ball_j = game_.ball_y_ * (float)SCR_HEIGHT;

        const int paddle_i = game_.paddle_x_ * (float)SCR_WIDTH;

        game_.renderer_.drawToBackBuffer(ball_i, ball_j, "*");
        game_.renderer_.drawToBackBuffer(paddle_i, 0, "----");

        game_.renderer_.render();

        //drawAll(); // draw to back buffer

        //render();  // render back buffer to screen

        Sleep(5);
    }
}
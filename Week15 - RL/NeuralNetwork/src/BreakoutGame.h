/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////
/

#pragma once

#include <stdlib.h>
#include <time.h>
#include "Vector2D.h"
#include "VectorND.h"
#include "ConsoleGame.h"

class BreakoutGame
{
public:
    float paddle_x_;    
    float ball_x_, ball_y_;
    float ball_vel_x_, ball_vel_y_;

    const float wall_thickness_;
    const float paddle_width_;

    VectorND<float> state_buffer_;

    ConsoleGame  renderer_;

    bool compat_state_;

    BreakoutGame()
        : wall_thickness_(0.05f), paddle_width_(0.3f)
    {
        compat_state_ = false;   // do not use convolutional image input

        srand((int)time(NULL));

        paddle_x_ = 0.5f;

        restart();

        state_buffer_.initialize(getNumStateVariables(), true);
    }

    int getNumStateVariables()
    {
        if(compat_state_ == true) return 3; // paddle_x, ball_x/y
        else
        {
            return SCR_WIDTH * SCR_HEIGHT;
        }
    }

    const VectorND<float>& getStateBuffer() // update state buffer and return it
    {
        if (compat_state_ == true)
        {
            state_buffer_[0] = paddle_x_;
            state_buffer_[1] = ball_x_;
            state_buffer_[2] = ball_y_;
        }
        else
        {
            for (int j = 0; j < SCR_HEIGHT; j++)
                for (int i = 0; i < SCR_WIDTH; i++)
                {
                    float value = 0.0;

                    if (renderer_.front_buffer[j][i] == '*') value = 1.0;
                    else if (renderer_.front_buffer[j][i] == '-') value = 0.5;

                    state_buffer_[i + SCR_WIDTH*j] = value;
                }
        }

        return state_buffer_;
    }

	void printStateBuffer()
	{
		for (int j = 0; j < SCR_HEIGHT; j++)
		{
			for (int i = 0; i < SCR_WIDTH; i++)
			{
				if (renderer_.front_buffer[j][i] == '*') std::cout << "1";
				else if (renderer_.front_buffer[j][i] == '-') std::cout << "0";
				else std::cout << " ";
			}
			std::cout << std::endl;
		}
	}

    void restart()
    {
        ball_x_ = 0.5;
        ball_y_ = 0.5;

        ball_vel_x_ = (float)rand() / (float)RAND_MAX - 0.5f;
        ball_vel_y_ = -0.5;

        Vector2D<float> temp(ball_vel_x_, ball_vel_y_);
        temp.normalize();
        ball_vel_x_ = temp.x_;
        ball_vel_y_ = temp.y_;
    }

    void movePad(const float& dx)
    {
        paddle_x_ += dx;

        if (paddle_x_ < 0.0f) paddle_x_ = 0.0f;
        else if (paddle_x_ > 1.0f - paddle_width_) paddle_x_ = 1.0f - paddle_width_;
    }

    void updateBall(const float& dt, float& reward)
    {
        reward = 0.0f;

        if (ball_x_ < wall_thickness_ && ball_vel_x_ < 0.0)
        {
            ball_vel_x_ = -ball_vel_x_;
        }
        else if (ball_x_ > 1.0 - wall_thickness_ && ball_vel_x_ > 0.0)
        {
            ball_vel_x_ = -ball_vel_x_;
        }

        if (ball_y_ < wall_thickness_ && ball_vel_y_ < 0.0)
        {
            if (ball_x_ >= paddle_x_ && ball_x_ <= paddle_x_ + paddle_width_)
            {
                ball_vel_y_ = -ball_vel_y_;
 //               ball_vel_x_ *= (0.8f + 0.4f * (float)rand() / (float)RAND_MAX);
                ball_vel_x_ += ((float)rand() / (float)RAND_MAX - 0.5f) * 0.2f;

				restart(); // for faster training

                reward = 1.0f;
            }
            else
            {
                restart();

                reward = -1.0f;

                return;
            }
        }

        if (ball_y_ > 1.0 - wall_thickness_ && ball_vel_y_ > 0.0)
        {
            ball_vel_y_ = -ball_vel_y_;
        }

        ball_x_ += ball_vel_x_ * dt;
        ball_y_ += ball_vel_y_ * dt;


    }
};

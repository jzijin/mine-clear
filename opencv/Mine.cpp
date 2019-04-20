#include "Mine.h"
Mime::Mime(string wname)
{
	grid = vector<vector<int>>(ROW, vector<int>(COL));
	marked = vector<vector<int>>(ROW, vector<int>(COL));
	flag = vector<vector<int>>(ROW, vector<int>(COL));
	count_flag = vector<vector<int>>(ROW, vector<int>(COL));
	is_pushed = vector<vector<int>>(ROW, vector<int>(COL));
	count = 0;

	// 设置窗口的名称
	window_name = wname;

	// 创建窗口
	namedWindow(window_name);

	// 设置img
	img = Mat(WIDTH, HEIGHT, CV_8UC1, Scalar(100, 100, 100));

	imshow(window_name, img);

	// 画棋盘
	drawLine(window_name, img);

	// 初始化地雷和棋盘
	init_mime();

	// 设置回调函数
	setMouseCallback(window_name, Mime::onMouse, this);
}

void Mime::drawLine(string windowname, Mat name)
{
	// 画竖线 
	for (int i = 0; i <= ROW; ++i)
	{
		line(name, Point(i*ROW_STEP, 0), Point(i*ROW_STEP, HEIGHT), Scalar(0, 0, 0));
		
	}
	// 画横线
	for (int j = 0; j <= COL; ++j)
	{
		line(name, Point(0, j*COL_STEP), Point(WIDTH, j*COL_STEP), Scalar(0, 0, 0));
	}
	imshow(windowname, name);
}

void Mime::init_mime()
{
	default_random_engine engine(time(nullptr)); // 设置随机数种子
	uniform_int_distribution<unsigned> dis(0, ROW - 1);
	uniform_int_distribution<unsigned> dis2(0, COL - 1);
	int total = 0; // 记录地雷的个数
	while (1)   // 随机生成指定个数的地雷
	{
		auto row = dis(engine);
		auto col = dis2(engine);

		if (!marked[row][col])
		{
			marked[row][col] = 1;			
			++total;
		}
		if (total == NUM) // 生成的地雷数目等于指定的地雷数目 退出
			break;
	}

	// 初始化grid
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			if (marked[i][j] == 1)
			{
				grid[i][j] = marked[i][j];
				//continue;
			}
			// 计算以此格点周围的地雷数
			else if (i == 0 && j == 0)
			{
				grid[i][j] = marked[i + 1][j] + marked[i + 1][j + 1] + marked[i][j + 1];
			}
			else if (i == 0 && j == COL - 1)
			{
				grid[i][j] = marked[i][j - 1] + marked[i + 1][j - 1] + marked[i + 1][j];
			}
			else if (i == ROW - 1 && j == 0)
			{
				grid[i][j] = marked[i][j + 1] + marked[i - 1][j] + marked[i - 1][j + 1];
			}
			else if (i == ROW - 1 && j == COL - 1)
			{
				grid[i][j] = marked[i][j - 1] + marked[i - 1][j] + marked[i - 1][j - 1];
			}
			else if (i == 0)
			{
				grid[i][j] = marked[i][j - 1] + marked[i][j + 1] + marked[i + 1][j] + marked[i + 1][j - 1] + marked[i + 1][j + 1];
			}
			else if (i == ROW - 1)
			{
				grid[i][j] = marked[i][j - 1] + marked[i][j + 1] + marked[i - 1][j] + marked[i - 1][j - 1] + marked[i - 1][j + 1];
			}
			else if (j == 0)
			{
				grid[i][j] = marked[i][j + 1] + marked[i - 1][j] + marked[i + 1][j] + marked[i + 1][j + 1] + marked[i - 1][j + 1];
			}
			else if (j == COL - 1)
			{
				grid[i][j] = marked[i][j - 1] + marked[i - 1][j] + marked[i + 1][j] + marked[i + 1][j - 1] + marked[i - 1][j - 1];
			}
			else
			{
				grid[i][j] = marked[i - 1][j] + marked[i - 1][j - 1] + marked[i - 1][j + 1] + marked[i + 1][j]
					+ marked[i + 1][j + 1] + marked[i + 1][j - 1] + marked[i][j + 1] + marked[i][j - 1];
			}



		}
	}
}

void Mime::onMouse(int events, int x, int y, int, void* userdata)
{
	Mime* temp = reinterpret_cast<Mime*>(userdata);
	temp->_on_Mouse(events, x, y);
}

void Mime::lbutton_click(int row, int col)
{
	if (!marked[row][col] && !flag[row][col]) // 如果不是地雷 且还未曾被点击
	{
		++count; // 点中格子的加上1
		// 如果已经清除的格子数目 等于总的格子数目 那么就说明扫除了所有的地雷
		if (count == ROW * COL)
			message(1);

		// 2 代表已经点击过
		flag[row][col] = 2;
		is_pushed[row][col] = 1;
		putText(img, to_string(grid[row][col]), Point(row * ROW_STEP + ROW_STEP/2, col * COL_STEP + COL_STEP/2), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0));
		imshow(window_name, img);

		// 点到的是零 直接调用mbutton_click
		if (grid[row][col] == 0)
			mbutton_click(row, col);
	}
	else if (!flag[row][col]) // 如果还没被点击
	{
		// 点中地雷
		for (auto i = 0; i < ROW; ++i)
		{
			for (auto j = 0; j < COL; ++j)
			{
				if (marked[i][j])
				{
					// 画出所有的地雷
					line(img, Point(i * ROW_STEP, j * COL_STEP), Point(i * ROW_STEP + ROW_STEP, j * COL_STEP + COL_STEP), Scalar(0, 0, 0));
					line(img, Point(i * ROW_STEP + ROW_STEP, j * COL_STEP), Point(i * ROW_STEP, j * COL_STEP + COL_STEP), Scalar(0, 0, 0));
				}

			}
		}
		imshow(window_name, img);
		// 点中地雷游戏失败 显示失败信息
		message(0);
	}

}
void Mime::rbutton_click(int row, int col)
{
	if (!flag[row][col])
	{
		// 画出标记形状
		line(img, Point(row * ROW_STEP + ROW_STEP/2, col * COL_STEP), Point(row * ROW_STEP + ROW_STEP/2, col * COL_STEP + COL_STEP), Scalar(0, 0, 0));
		line(img, Point(row * ROW_STEP, col * COL_STEP + COL_STEP/2), Point(row * ROW_STEP + ROW_STEP, col * COL_STEP + COL_STEP/2), Scalar(0, 0, 0));

		// 设置flag  1 代表用户标记为地雷
		flag[row][col] = 1;

		// 主要是方便计算 格点周围共有几个已经被用户标记为地雷的格点。
		count_flag[row][col] = 1;

		// is_pushed 1 代表不需要加到 mbutton 中的队列中
		is_pushed[row][col] = 1;

		// 已经清楚不是地雷和已经标记了地雷的格点加1；
		++count;

		// 如果已经知道不是地雷和已经标记为地雷的格点 等于总格点 那么用户赢得了胜利。
		if (count == ROW * COL)
			message(1);
	}
	else if (flag[row][col] == 1)  // 取消找到地雷的标记
	{
		// 设置成和背景颜色一样就看不出来了
		line(img, Point(row * ROW_STEP + ROW_STEP / 2, col * COL_STEP), Point(row * ROW_STEP + ROW_STEP / 2, col * COL_STEP + COL_STEP), Scalar(100, 100, 100));
		line(img, Point(row * ROW_STEP, col * COL_STEP + COL_STEP / 2), Point(row * ROW_STEP + ROW_STEP, col * COL_STEP + COL_STEP / 2), Scalar(100, 100, 100));
		
		// flag重新设置为未点击状态
		flag[row][col] = 0;

		// is_pushed 0 可以加入mbutton中的队列
		is_pushed[row][col] = 0;

		// 主要是方便计算 格点周围共有几个已经被用户标记为地雷的格点。
		count_flag[row][col] = 0;

		// 取消标记 已经清楚的格子数目减1
		--count;

	}
	imshow(window_name, img);
}
void Mime::mbutton_click(int row, int col)
{
	if (flag[row][col])
	{
		get_next_points(row, col);
		// 广度优先搜索
		while (!next_points.empty())
		{
			Point tmp = next_points.front();
			next_points.pop();
			lbutton_click(tmp.x, tmp.y);
			// 得到这个格点的周围格点
			get_next_points(tmp.x, tmp.y);
		}
	}
}


void Mime::get_next_points(int row, int col)
{
	// 得到以此点为中心的地雷数目
	int mime_num = grid[row][col];
	// 统计以此点为中心的周围格子的标志总数
	int flag_count = 0;

	if (row == 0 && col == 0)
	{
		flag_count = count_flag[row + 1][col] + count_flag[row + 1][col + 1] + count_flag[row][col + 1];
		
		if (flag_count == mime_num)
		{
			if (!is_pushed[row + 1][col])
			{
				next_points.push(Point(row + 1, col));
				is_pushed[row + 1][col] = 1;
			}
			if (!is_pushed[row + 1][col+1])
			{
				next_points.push(Point(row + 1, col + 1));
				is_pushed[row + 1][col+1] = 1;
			}
			if (!is_pushed[row][col+1])
			{
				next_points.push(Point(row, col + 1));
				is_pushed[row][col+1] = 1;
			}			
		}
	}
	else if (row == 0 && col == COL - 1)
	{
		flag_count = count_flag[row][col - 1] + count_flag[row + 1][col - 1] + count_flag[row + 1][col];
		if (flag_count == mime_num) {

			if (!is_pushed[row][col - 1])
			{
				next_points.push(Point(row, col - 1));
				is_pushed[row][col - 1] = 1;
			}

			if (!is_pushed[row+1][col - 1])
			{
				next_points.push(Point(row + 1, col - 1));
				is_pushed[row+1][col - 1] = 1;
			}

			if (!is_pushed[row+1][col])
			{
				next_points.push(Point(row + 1, col));
				is_pushed[row + 1][col] = 1;
			}			
		}
	}
	else if (row == ROW - 1 && col == 0)
	{
		flag_count = count_flag[row][col + 1] + count_flag[row - 1][col] + count_flag[row - 1][col + 1];
		if (flag_count == mime_num) {

			if (!is_pushed[row][col + 1])
			{
				next_points.push(Point(row, col + 1));
				is_pushed[row][col+1] = 1;
			}

			if (!is_pushed[row-1][col])
			{
				next_points.push(Point(row - 1, col));
				is_pushed[row-1][col] = 1;
			}

			if (!is_pushed[row - 1][col+1])
			{
				next_points.push(Point(row - 1, col + 1));
				is_pushed[row - 1][col+1] = 1;
			}
		}
	}
	else if (row == ROW - 1 && col == COL - 1)
	{
		flag_count = count_flag[row][col - 1] + count_flag[row - 1][col] + count_flag[row - 1][col - 1];
		if (flag_count == mime_num) {

			if (!is_pushed[row][col - 1])
			{
				next_points.push(Point(row, col - 1));
				is_pushed[row][col - 1] = 1;
			}

			if (!is_pushed[row-1][col])
			{
				next_points.push(Point(row - 1, col));
				is_pushed[row-1][col] = 1;
			}
			
			if (!is_pushed[row - 1][col-1])
			{
				next_points.push(Point(row - 1, col - 1));
				is_pushed[row - 1][col-1] = 1;
			}			
		}
	}
	else if (row == 0)
	{
		flag_count = count_flag[row][col - 1] + count_flag[row][col + 1] + count_flag[row + 1][col] + count_flag[row + 1][col - 1] + count_flag[row + 1][col + 1];
		if (flag_count == mime_num) {

			if (!is_pushed[row][col - 1])
			{
				next_points.push(Point(row, col - 1));
				is_pushed[row][col - 1] = 1;
			}
			
			if (!is_pushed[row][col + 1])
			{
				next_points.push(Point(row, col + 1));
				is_pushed[row][col + 1] = 1;
			}
			if (!is_pushed[row+1][col])
			{
				next_points.push(Point(row + 1, col));
				is_pushed[row+1][col] = 1;
			}

			if (!is_pushed[row + 1][col-1])
			{
				next_points.push(Point(row + 1, col - 1));
				is_pushed[row + 1][col-1] = 1;
			}

			if (!is_pushed[row + 1][col + 1])
			{
				next_points.push(Point(row + 1, col + 1));
				is_pushed[row + 1][col + 1] = 1;
			}
		}
	}
	else if (row == ROW - 1)
	{
		flag_count = count_flag[row][col - 1] + count_flag[row][col + 1] + count_flag[row - 1][col] + count_flag[row - 1][col - 1] + count_flag[row - 1][col + 1];
		if (flag_count == mime_num) {
			if (!is_pushed[row][col - 1])
			{
				next_points.push(Point(row, col - 1));
				is_pushed[row][col - 1] = 1;
			}

			if (!is_pushed[row][col + 1])
			{
				next_points.push(Point(row, col + 1));
				is_pushed[row][col + 1] = 1;
			}

			if (!is_pushed[row-1][col])
			{
				next_points.push(Point(row - 1, col));
				is_pushed[row-1][col] = 1;
			}

			if (!is_pushed[row - 1][col-1])
			{
				next_points.push(Point(row - 1, col - 1));
				is_pushed[row - 1][col-1] = 1;
			}

			if (!is_pushed[row - 1][col + 1])
			{
				next_points.push(Point(row - 1, col + 1));
				is_pushed[row - 1][col + 1] = 1;
			}
		}
	}
	else if (col == 0)
	{
		flag_count = count_flag[row][col + 1] + count_flag[row - 1][col] + count_flag[row + 1][col] + count_flag[row + 1][col + 1] + count_flag[row - 1][col + 1];
		if (flag_count == mime_num) {
			if (!is_pushed[row][col + 1])
			{
				next_points.push(Point(row, col + 1));
				is_pushed[row][col + 1] = 1;
			}

			if (!is_pushed[row-1][col])
			{
				next_points.push(Point(row - 1, col));
				is_pushed[row-1][col] = 1;
			}

			if (!is_pushed[row + 1][col])
			{
				next_points.push(Point(row + 1, col));
				is_pushed[row + 1][col] = 1;
			}

			if (!is_pushed[row + 1][col + 1])
			{
				next_points.push(Point(row + 1, col + 1));
				is_pushed[row + 1][col + 1] = 1;
			}

			if (!is_pushed[row - 1][col + 1])
			{
				next_points.push(Point(row - 1, col + 1));
				is_pushed[row - 1][col + 1] = 1;
			}
		}
	}
	else if (col == COL - 1)
	{
		flag_count = count_flag[row][col - 1] + count_flag[row - 1][col] + count_flag[row + 1][col] + count_flag[row + 1][col - 1] + count_flag[row - 1][col - 1];
		if (flag_count == mime_num) {

			if (!is_pushed[row ][col - 1])
			{
				next_points.push(Point(row, col - 1));
				is_pushed[row ][col - 1] = 1;
			}

			if (!is_pushed[row-1][col])
			{
				next_points.push(Point(row - 1, col));
				is_pushed[row-1][col] = 1;
			}

			if (!is_pushed[row + 1][col])
			{
				next_points.push(Point(row + 1, col));
				is_pushed[row + 1][col] = 1;
			}

			if (!is_pushed[row + 1][col-1])
			{
				next_points.push(Point(row + 1, col - 1));
				is_pushed[row + 1][col-1] = 1;
			}

			if (!is_pushed[row - 1][col - 1])
			{
				next_points.push(Point(row - 1, col - 1));
				is_pushed[row - 1][col - 1] = 1;
			}		}
	}
	else
	{
		flag_count = count_flag[row - 1][col] + count_flag[row - 1][col - 1] + count_flag[row - 1][col + 1] + count_flag[row + 1][col]
			+ count_flag[row + 1][col + 1] + count_flag[row + 1][col - 1] + count_flag[row][col + 1] + count_flag[row][col - 1];
		if (flag_count == mime_num) {

			if (!is_pushed[row - 1][col])
			{
				next_points.push(Point(row - 1, col));
				is_pushed[row - 1][col] = 1;
			}

			if (!is_pushed[row - 1][col-1])
			{
				next_points.push(Point(row - 1, col - 1));
				is_pushed[row - 1][col-1] = 1;
			}

			if (!is_pushed[row - 1][col + 1])
			{
				next_points.push(Point(row - 1, col + 1));
				is_pushed[row - 1][col + 1] = 1;
			}

			if (!is_pushed[row + 1][col])
			{
				next_points.push(Point(row + 1, col));
				is_pushed[row + 1][col ] = 1;
			}

			if (!is_pushed[row + 1][col+1])
			{
				next_points.push(Point(row + 1, col + 1));
				is_pushed[row + 1][col+1] = 1;
			}

			if (!is_pushed[row + 1][col - 1])
			{
				next_points.push(Point(row + 1, col - 1));
				is_pushed[row + 1][col - 1] = 1;
			}
			
			if (!is_pushed[row][col + 1])
			{
				next_points.push(Point(row, col + 1));
				is_pushed[row][col + 1] = 1;
			}

			if (!is_pushed[row][col - 1])
			{
				next_points.push(Point(row, col - 1));
				is_pushed[row][col - 1] = 1;
			}
		}
	}
}

void Mime::message(int succ)
{
	if (!succ)
	{

		putText(img, "You are lose!!!", Point(0, HEIGHT/2), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));

	}
	else
	{
		putText(img, "You win, Congratulation!!!", Point(0, HEIGHT/2), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));

	}
	for (auto i = 0; i < flag.size(); ++i)
	{
		for (auto j = 0; j < flag[0].size(); ++j)
		{
			// 把所有格点设置为不可点击状态。
			flag[i][j] = 1;
		}
	}
	imshow(window_name, img);

}
void Mime::_on_Mouse(int events, int x, int y)
{
	int row = x / ROW_STEP;
	int col = y / COL_STEP;
	switch (events)
	{
		// 鼠标左键点击事件
	case EVENT_LBUTTONDOWN:
		lbutton_click(row, col);
		break;

		// 鼠标右键点击事件
	case EVENT_RBUTTONDOWN:
		rbutton_click(row, col);
		break;

		// 鼠标中键点击事件
	case EVENT_MBUTTONDOWN:
		mbutton_click(row, col);
		break;
	}
}
Mime::~Mime()
{
}

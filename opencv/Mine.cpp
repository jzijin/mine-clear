#include "Mine.h"
Mime::Mime(string wname)
{
	grid = vector<vector<int>>(ROW, vector<int>(COL));
	marked = vector<vector<int>>(ROW, vector<int>(COL));
	flag = vector<vector<int>>(ROW, vector<int>(COL));
	count_flag = vector<vector<int>>(ROW, vector<int>(COL));
	is_pushed = vector<vector<int>>(ROW, vector<int>(COL));
	count = 0;

	// ���ô��ڵ�����
	window_name = wname;

	// ��������
	namedWindow(window_name);

	// ����img
	img = Mat(WIDTH, HEIGHT, CV_8UC1, Scalar(100, 100, 100));

	imshow(window_name, img);

	// ������
	drawLine(window_name, img);

	// ��ʼ�����׺�����
	init_mime();

	// ���ûص�����
	setMouseCallback(window_name, Mime::onMouse, this);
}

void Mime::drawLine(string windowname, Mat name)
{
	// ������ 
	for (int i = 0; i <= ROW; ++i)
	{
		line(name, Point(i*ROW_STEP, 0), Point(i*ROW_STEP, HEIGHT), Scalar(0, 0, 0));
		
	}
	// ������
	for (int j = 0; j <= COL; ++j)
	{
		line(name, Point(0, j*COL_STEP), Point(WIDTH, j*COL_STEP), Scalar(0, 0, 0));
	}
	imshow(windowname, name);
}

void Mime::init_mime()
{
	default_random_engine engine(time(nullptr)); // �������������
	uniform_int_distribution<unsigned> dis(0, ROW - 1);
	uniform_int_distribution<unsigned> dis2(0, COL - 1);
	int total = 0; // ��¼���׵ĸ���
	while (1)   // �������ָ�������ĵ���
	{
		auto row = dis(engine);
		auto col = dis2(engine);

		if (!marked[row][col])
		{
			marked[row][col] = 1;			
			++total;
		}
		if (total == NUM) // ���ɵĵ�����Ŀ����ָ���ĵ�����Ŀ �˳�
			break;
	}

	// ��ʼ��grid
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			if (marked[i][j] == 1)
			{
				grid[i][j] = marked[i][j];
				//continue;
			}
			// �����Դ˸����Χ�ĵ�����
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
	if (!marked[row][col] && !flag[row][col]) // ������ǵ��� �һ�δ�������
	{
		++count; // ���и��ӵļ���1
		// ����Ѿ�����ĸ�����Ŀ �����ܵĸ�����Ŀ ��ô��˵��ɨ�������еĵ���
		if (count == ROW * COL)
			message(1);

		// 2 �����Ѿ������
		flag[row][col] = 2;
		is_pushed[row][col] = 1;
		putText(img, to_string(grid[row][col]), Point(row * ROW_STEP + ROW_STEP/2, col * COL_STEP + COL_STEP/2), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0));
		imshow(window_name, img);

		// �㵽������ ֱ�ӵ���mbutton_click
		if (grid[row][col] == 0)
			mbutton_click(row, col);
	}
	else if (!flag[row][col]) // �����û�����
	{
		// ���е���
		for (auto i = 0; i < ROW; ++i)
		{
			for (auto j = 0; j < COL; ++j)
			{
				if (marked[i][j])
				{
					// �������еĵ���
					line(img, Point(i * ROW_STEP, j * COL_STEP), Point(i * ROW_STEP + ROW_STEP, j * COL_STEP + COL_STEP), Scalar(0, 0, 0));
					line(img, Point(i * ROW_STEP + ROW_STEP, j * COL_STEP), Point(i * ROW_STEP, j * COL_STEP + COL_STEP), Scalar(0, 0, 0));
				}

			}
		}
		imshow(window_name, img);
		// ���е�����Ϸʧ�� ��ʾʧ����Ϣ
		message(0);
	}

}
void Mime::rbutton_click(int row, int col)
{
	if (!flag[row][col])
	{
		// ���������״
		line(img, Point(row * ROW_STEP + ROW_STEP/2, col * COL_STEP), Point(row * ROW_STEP + ROW_STEP/2, col * COL_STEP + COL_STEP), Scalar(0, 0, 0));
		line(img, Point(row * ROW_STEP, col * COL_STEP + COL_STEP/2), Point(row * ROW_STEP + ROW_STEP, col * COL_STEP + COL_STEP/2), Scalar(0, 0, 0));

		// ����flag  1 �����û����Ϊ����
		flag[row][col] = 1;

		// ��Ҫ�Ƿ������ �����Χ���м����Ѿ����û����Ϊ���׵ĸ�㡣
		count_flag[row][col] = 1;

		// is_pushed 1 ������Ҫ�ӵ� mbutton �еĶ�����
		is_pushed[row][col] = 1;

		// �Ѿ�������ǵ��׺��Ѿ�����˵��׵ĸ���1��
		++count;

		// ����Ѿ�֪�����ǵ��׺��Ѿ����Ϊ���׵ĸ�� �����ܸ�� ��ô�û�Ӯ����ʤ����
		if (count == ROW * COL)
			message(1);
	}
	else if (flag[row][col] == 1)  // ȡ���ҵ����׵ı��
	{
		// ���óɺͱ�����ɫһ���Ϳ���������
		line(img, Point(row * ROW_STEP + ROW_STEP / 2, col * COL_STEP), Point(row * ROW_STEP + ROW_STEP / 2, col * COL_STEP + COL_STEP), Scalar(100, 100, 100));
		line(img, Point(row * ROW_STEP, col * COL_STEP + COL_STEP / 2), Point(row * ROW_STEP + ROW_STEP, col * COL_STEP + COL_STEP / 2), Scalar(100, 100, 100));
		
		// flag��������Ϊδ���״̬
		flag[row][col] = 0;

		// is_pushed 0 ���Լ���mbutton�еĶ���
		is_pushed[row][col] = 0;

		// ��Ҫ�Ƿ������ �����Χ���м����Ѿ����û����Ϊ���׵ĸ�㡣
		count_flag[row][col] = 0;

		// ȡ����� �Ѿ�����ĸ�����Ŀ��1
		--count;

	}
	imshow(window_name, img);
}
void Mime::mbutton_click(int row, int col)
{
	if (flag[row][col])
	{
		get_next_points(row, col);
		// �����������
		while (!next_points.empty())
		{
			Point tmp = next_points.front();
			next_points.pop();
			lbutton_click(tmp.x, tmp.y);
			// �õ����������Χ���
			get_next_points(tmp.x, tmp.y);
		}
	}
}


void Mime::get_next_points(int row, int col)
{
	// �õ��Դ˵�Ϊ���ĵĵ�����Ŀ
	int mime_num = grid[row][col];
	// ͳ���Դ˵�Ϊ���ĵ���Χ���ӵı�־����
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
			// �����и������Ϊ���ɵ��״̬��
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
		// ����������¼�
	case EVENT_LBUTTONDOWN:
		lbutton_click(row, col);
		break;

		// ����Ҽ�����¼�
	case EVENT_RBUTTONDOWN:
		rbutton_click(row, col);
		break;

		// ����м�����¼�
	case EVENT_MBUTTONDOWN:
		mbutton_click(row, col);
		break;
	}
}
Mime::~Mime()
{
}

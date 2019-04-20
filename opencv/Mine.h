#pragma once
#include <iostream>  
#include <string>
#include <vector>
#include <queue>
#include <random>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>  
using namespace std;
using namespace cv;
constexpr auto WIDTH = 600;
constexpr auto HEIGHT = 600; // TODO: ���������С���Զ���Ӧ�õ��ȽϺõ�Ч��
//constexpr auto MIME = 10; // 10 ������ִ�����;
constexpr auto ROW = 10;
constexpr auto COL = 10; // �����к���;
constexpr auto NUM = 20; // ��Ҫ���ɼ�����
constexpr auto ROW_STEP = WIDTH / ROW;
constexpr auto COL_STEP = HEIGHT / COL;

class Mime
{
public:
	Mime(string wname);
	~Mime();

private:
	int count; // ͳ�Ƶ�ĿǰΪֹ һ���ж��ٸ����ӱ����
	vector<vector<int>> flag;
	vector<vector<int>> count_flag;
	vector<vector<int>> is_pushed; // ͳ���������Ƿ񱻼ӽ�����
	
	queue<Point> next_points; // ��¼�м��������һЩ�ڵ�
	//vector<Point> mime; // ��¼���е��׵�λ��
	vector<vector<int>> grid; // ��������
	vector<vector<int>> marked; // ��־�Ƿ���һ����
	string window_name; // ��������
	Mat img;  // ͼ�ξ���

	// ������
	void drawLine(string windowname, Mat name); 

	// ��ʼ�����̺͵���
	void init_mime();

	// ����¼��ص�����
	void _on_Mouse(int events, int x, int y);
	static void onMouse(int events, int x, int y, int, void* userdata);
	void lbutton_click(int row, int col);
	void rbutton_click(int row, int col);
	void mbutton_click(int row, int col);
	void get_next_points(int row, int col);
	void message(int succ);
};



#include "camera.h"

/*
* c=(0,0,0), r=distance ���� �� ���� 0.001% ������ �����δ�
* x^2+y^2+z^2=r^2
* {cos	-sin}
* {sin	cos	}
*/
void camera::setLocation(float movx, float movy) {
	angle_h += 2 * M_PI*0.001*movx;
	angle_v += 2 * M_PI*0.001*movy;
	if (angle_h > 2 * M_PI)
		angle_h -= 2 * M_PI;
	else if (angle_h <= 0)
		angle_h += 2 * M_PI;
	if (angle_v > 2 * M_PI)
		angle_v -= 2 * M_PI;
	else if (angle_v <= 0)
		angle_v += 2 * M_PI;

	x = distance * cos(angle_v) * cos(angle_h);
	y = distance * cos(angle_v) * sin(angle_h);
	z = distance * sin(angle_v);

}

/*
* ��ġ�� �������� distance�� �ǵ��� �̵�
* x' = x*t, y' = y*t, z' = z*t ���� ���� ������ ����
*/
void camera::setDistance(float dd) {
	distance += dd;
	if (distance <= 1)
		distance = 1;
	
	double t = sqrt(pow(distance, 2) / (pow(x, 2) + pow(y, 2) + pow(z, 2)));
	int sign = getNear(t);
	x = sign*x*t;
	y = sign*y*t;
	z = sign*z*t;

}

int camera::getNear(double t) {
	double dist_plus = sqrt(pow((1 - t)*x, 2) + pow((1 - t)*y, 2) + pow((1 - t)*z, 2));
	double dist_minus = sqrt(pow((1 + t)*x, 2) + pow((1 + t)*y, 2) + pow((1 + t)*z, 2));

	if (dist_plus >= dist_minus)
		return -1;
	else
		return 1;
}
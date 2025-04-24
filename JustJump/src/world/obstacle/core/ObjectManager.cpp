#include "Global.h"
#include "world/obstacle/core/ObjectManager.h"
#include <fstream>


//������Ʈ�� �÷��̾� �浹üũ 1�̸� �δ�ħ
bool collp2w(PLAYER player, OBJECT object)
//----------------------------------------
{
	int adjust = 10;
	//�� 101�� �������ĸ� ���Ǻ��ٴ� ��ֹ��� �켱�����⶧����
	if (101<=object.getType() &&object.getType()<301) { //��ֹ��϶��� �÷��̾� �׸������ ��Ʈ�ڽ����ȴ�
		if (object.getType() == 106 || object.getType() == 107)
		{
			if (player.getx() + player.getw() < object.getX()+object.getmx() || player.getx() - player.getw() > object.getX() + object.getmx() + object.getW()) return 0;
			if (player.gety() + player.geth() < object.getY() + object.getmy() || player.gety() - player.geth() > object.getY() + object.getmy() + object.getH()) return 0;
		}
		else
		{
			if (player.getx() + player.getw() < object.getX() || player.getx() - player.getw() > object.getX() + object.getW()) return 0;
			if (player.gety() + player.geth() < object.getY() || player.gety() - player.geth() > object.getY() + object.getH()) return 0;
		}
		

		return 1;
	} 
	else if (301 <= object.getType() && object.getType() < 401)	//����,���ٰ��� ���� ������ϴ� ������ ���δ� �÷��̾� �߱��� ��ƾ��ϰ� �Ʒ��δ� �÷��̾� �������� ������ ������ ���������� �־���ϹǷ� ���� ���ϰ� �ش�
	{

		if (player.gety() + player.geth() < object.getY() || player.gety() - player.geth() > object.getY() + object.getH()) return 0;	//�ϴܸ��� ������� ����

		if (object.getX() < player.getx() && player.getx() < object.getX() + object.getW())	//�������� �׷��� �� �β���� �̾ȿ������� cehck
		{
			if (UPkey == true)//����� Ư���ϰ� �ö󰡸� �ö󰡴��� üũ�� ���̳����Ѵ�.
			{

				if (player.gety() + player.geth() <= object.getY())	//�ö����� �Ʒ�Ű�� �����ϸ� �浹üũ x �ȱ׷��� �����ؼ� ��� �ٿ��Ŵ޸��� ����
					return 0;
				if (player.gety() < object.getY() + object.getH())
					return 1;
			}
			else if (DOWNkey == true)
			{

				if (player.gety() + player.geth() <= object.getY())
					return 1;
			}

			if (player.gety() + player.geth() <= object.getY() || player.gety() < object.getY() + object.getH())
				return 1;
		}
		
		return 0;
	}
	else if (object.getType() == 1)	//���ٴ��϶�
	{
		if (object.getX() <= player.getx() && player.getx() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.gety() + player.geth())
			{
				return 1;
			}
		}
	}
	else if (object.getType() <= 100) {	//�÷����϶��� �÷��̾� ������ ��Ʈ�ڽ����ȴ�
		if (object.getX() <= player.getx() && player.getx() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.gety() + player.geth() && player.gety() + player.geth() <= object.getY() + adjust)
			{
				return 1;
				//����
			}
		}
	}

	return 0;
}


//�÷��̾�� ������Ʈ�� ��ȣ�ۿ� �Ǵ��ϰ� �׿��°� �ٲ���
void adjustPlayer(PLAYER& player, OBJECT* obj, MAP& m, int& ocount, HINSTANCE g_hinst, Sound& sound)
{
	int check_coll = 0;	//�ϳ��� �ε������� �Ǻ��ϱ�����
	if (player.getx() - player.getw() < 0)
	{
		player.setx(player.getw());
		player.setCMD_move(0);
	}
	else if (player.getx() + player.getw() > 1023)
	{
		player.setx(1023 - player.getw());
		player.setCMD_move(0);
	}
	for (int i = 0; i < ocount; ++i)
	{
		//if (player.getstate() == 2)		//�������϶� ��� �ٴ��̶��� �浹 �����Ѵ�
		//	break;
		//if (player.getstate() == 3)		//������ 7�� �ƴϸ� �Ʒ��� �ȶ������ϱ�� ���� ���̱�,�������϶��� �浹ó�� ������
		//	break;
		if (collp2w(player, obj[i]))
		{
			check_coll++;	//�ϳ��� �ε������� coll�� �ö�
			if (obj[i].getType() < 101 && obj[i].getType()>0)			//�ٵ� �װ� ���ٴ��̿���?
			{

				if (player.getstate() == 7) //�������� ���϶� �ε����� ?
				{
					if (abs(player.getsavey() - player.gety()) > 200)	//�������޾ƾ��Ѵٸ�
					{
						if (player.getstealth() == 0)	//������ �ƴ϶��
						{
							player.setCMD_move(player.getdir());	//�����ִ¹������� ������ ������, �������µ� ���������� ������
							player.setstate(6);		//�ǰ����ΰ�
							player.hurt(sound);
							return;
						}
					}
					player.sety(obj[i].getY() - player.geth());//�������� y��ǥ �������ְ�

					if (LEFTkey == 0 && RIGHTkey == 0)	//�ٵ� �׿��߿��� �ƹ�Ű�� �ȴ������־��� ? 
						player.setCMD_move(false);	//�׷� ����������� ���°� ���ߵ������ش�.
					else if (LEFTkey == 1 && RIGHTkey == 1)
						player.setCMD_move(false);	//���ÿ� �������־�� �����ش�
					else if (LEFTkey == 1)	//������ ������ �������־���?
						player.setCMD_move(1);
					else if (RIGHTkey == 1)			//�׿�����ٲ��ش�
						player.setCMD_move(2);

					if (DOWNkey == true) {
						player.setstate(3);	//���̰��ִ� ���¿��ٸ� ��� ���̰��־���
						player.sety(player.gety() + 12);
						player.seth(player.geth() - 12);	//����� �����ٸ� �ٽ� ���̱���·� ������
					}
					else player.setstate(1);				//���̴��� �ƴϿ����� �����ε������� ������������
					player.setadjspd(0);			//�����������ӵ��� ���ѰŴ� �̰͵� ��������
					player.setCMD_hurt(0);			//���� ������� �ǰݾƴ�
					player.setCMD_ropehurt(0);		//���� ������� �ǰݾƴ�

					if (ROWSPEED != 3)		//ROWSPEED�� ���Ƿ� �ٲ��־��ٸ� ���� ������ �ʱ�ȭ�� ������� �����ش�
						ROWSPEED = 3;
				}

				if (obj[i].getType() == 4)
				{
					player.setx(player.getx() + beltspeed);
				}
				if (obj[i].getType() == 6)
				{
					player.setx(player.getx() - beltspeed);
				}
			}
			else if (obj[i].getType() >= 101 && obj[i].getType() <= 200)	//��ֹ��� �ε�����
			{

				if (obj[i].getType() == 101)	//��ö��
				{
					if (player.getstealth() == 0)	//������ �ƴ϶��
					{
						if (player.getstate() == 5 || player.getstate() == 8)
						{
							player.setCMD_ropehurt(1);
						}
						if (player.getstate()==3) //���̰��־��ٸ�
						{
							player.sety(player.gety() - 12);
							player.seth(player.geth() + 12);	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (player.getstate() == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (player.getCMD_move() == 1)
							{
								player.setspike_hurt(-8);	//8�� �������� ��
							}
							else if (player.getCMD_move() == 2)
							{
								player.setspike_hurt(8);	//8�� ���������ΰ�
							}

							player.setstealth(100);	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							player.setstate(6);		//�ǰ����ΰ�
						}
						player.hurt(sound);
					}
				}
				else if (obj[i].getType() == 102) //Break Pipe Left
				{
					//Copy and Paste is very good (Y Collapse)
					if (player.getstate() == 7) //�������� ���϶� �ε����� ?
					{
						player.sety(obj[i].getY() - player.geth());//�������� y��ǥ �������ְ�

						if (LEFTkey == 0 && RIGHTkey == 0)	//�ٵ� �׿��߿��� �ƹ�Ű�� �ȴ������־��� ? 
							player.setCMD_move(false);	//�׷� ����������� ���°� ���ߵ������ش�.
						else if (LEFTkey == 1 && RIGHTkey == 1)
							player.setCMD_move(false);	//���ÿ� �������־�� �����ش�
						else if (LEFTkey == 1)	//������ ������ �������־���?
							player.setCMD_move(1);
						else if (RIGHTkey == 1)			//�׿�����ٲ��ش�
							player.setCMD_move(2);

						player.setstate(1);				//�׸��� �����ε������� ������������
						player.setadjspd(0);			//�����������ӵ��� ���ѰŴ� �̰͵� ��������
						player.setCMD_hurt(0);			//���� ������� �ǰݾƴ�
						player.setCMD_ropehurt(0);		//���� ������� �ǰݾƴ�

						if (ROWSPEED != 3)		//ROWSPEED�� ���Ƿ� �ٲ��־��ٸ� ���� ������ �ʱ�ȭ�� ������� �����ش�
							ROWSPEED = 3;
					}
					//X Collapse
					if (player.getstate() == 1 || player.getstate() == 4) //Walking Collpse
					{
						if (obj[i].getY() < player.gety() - player.geth())
						{
							if (obj[i].getX() < player.getx() + player.getw()) //Left Collpse
							{
								player.setx(obj[i].getX() - player.getw());// x��ǥ �������ְ�
								player.setCMD_move(0);//Don't Move
							}
						}
					}
				}
				else if (obj[i].getType() == 103) //���� ����, ���ÿ� ����� ��� ���Ⱑ ������ �վ���  �������� �ǰ������� �ִ�.
				{
					if (obj[i].getindex() == 2) //���Ⱑ ������ �վ��������� �ǰ��� �߻��Ѵ�
					{
						if (player.getstealth() == 0)
						{
							if (player.getstate() == 5 || player.getstate() == 8)
							{
								player.setCMD_ropehurt(1);
							}
							if (player.getstate() == 3) //���̰��־��ٸ�
							{
								player.sety(player.gety() - 12);
								player.seth(player.geth() + 12);	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
							}
							if (player.getstate() == 7)
							{
								if (player.getdir() == 1 || player.getdir() == 2) //������ �������ΰ�
								{
									player.setspike_hurt(-8);
								}

								player.setstealth(100);
							}
							else {
								player.setCMD_move(1); //������ ������
								player.setstate(6);
							}
							player.hurt(sound);
						}
					}
				}
				else if (obj[i].getType() == 104) //Break Pipe Right
				{
					//Update Cooming Soon
				}
				else if (obj[i].getType() == 105) //Gas Right
				{
					//Update Cooming Soon
				}
				else if (obj[i].getType() == 106)
				{
					if (player.getstealth() == 0)	//������ �ƴ϶��
					{
						if (player.getstate() == 5 || player.getstate() == 8)
						{
							player.setCMD_ropehurt(1);
						}
						if (player.getstate() == 3) //���̰��־��ٸ�
						{
							player.sety(player.gety() - 12);
							player.seth(player.geth() + 12);	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (player.getstate() == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (player.getdir() == 1)
							{
								player.setspike_hurt(-8);	//8�� �������� ��
							}
							else if (player.getdir() == 2)
							{
								player.setspike_hurt(8);	//8�� ���������ΰ�
							}

							player.setstealth(100);	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							player.setCMD_move(player.getdir());
							player.setstate(6);		//�ǰ����ΰ�
						}
						player.hurt(sound);
					}
				}
				else if (obj[i].getType() == 107)
				{
					if (player.getstealth() == 0)	//������ �ƴ϶��
					{
						if (player.getstate() == 5 || player.getstate() == 8)
						{
							player.setCMD_ropehurt(1);
						}
						if (player.getstate() == 3) //���̰��־��ٸ�
						{
							player.sety(player.gety() - 12);
							player.seth(player.geth() + 12);	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (player.getstate() == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (player.getdir() == 1)
							{
								player.setspike_hurt(-8);	//8�� �������� ��
							}
							else if (player.getdir() == 2)
							{
								player.setspike_hurt(8);	//8�� ���������ΰ�
							}

							player.setstealth(100);	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							player.setCMD_move(player.getdir());
							player.setstate(6);		//�ǰ����ΰ�
						}
						player.hurt(sound);
					}
				}
			}
			else if (obj[i].getType() >= 201 && obj[i].getType() <= 300) //�÷��̾�� ��ȣ�ۿ��ϴ� ������Ʈ ex)��Ż
			{
				if (obj[i].getType() == 201) //Portal
				{
					if (UPkey == true)
					{
						m.setblack_t(50);
						/*m.CreateBlack(g_hinst);*/
						m.setmapnum(m.getmapnum() + 1);
						player.initPos();
						if (m.getmapnum() == 13) m.CreateMap(g_hinst);
						for (int j = 0; j < ocount; j++)
							obj[j].ResetObject();
						ocount = initObject(obj, m.getmapnum(), g_hinst);
						m.CreateMap(g_hinst);
						sound.setindex(m.getmapnum()-9);
						sound.System->update();
						sound.Channel[0]->stop();
						sound.System->playSound(
							sound.bgmSound[sound.getindex()],
							nullptr,
							false,
							&sound.Channel[0]
						);
						sound.Channel[1]->stop();
						sound.System->playSound(
							sound.effectSound[1],
							nullptr,
							false,
							&sound.Channel[1]
						);

						
						return;
					}
				}
			}
			else if (obj[i].getType() == 301) //Rope
			{
				if (player.getjumpignore() <= 0)
				{
					if (player.getCMD_ropehurt() == 0)	//�������� ������ �ٽ� ���� ��Ž
					{
						if (UPkey == true || DOWNkey == true)
						{
							if (DOWNkey == true && (player.getstate() == 2 || player.getstate() == 7))	//���߿��ְų� �������϶� �Ʒ�Ű�δ� �ٿ� ����������
								return;

							if (player.getstate() != 5 && player.getstate() != 8)	//�ٿ� �Ŵ޷����� �ʾҴٸ� �ٿ� �Ŵ޸��� ���¸� ������ش�. �̹̺پ��ִٸ� �����ʿ����
							{
								player.setstate(5);
								if (UPkey == true)
									player.setCMD_move(3);
								if (DOWNkey == true)
									player.setCMD_move(4);
								player.setx(obj[i].getX() + (obj[i].getW() / 2));
								if (DOWNkey == true)	//�̶��� ���׸���ƴ϶� ���� �Ʒ��� �������°��̹Ƿ� ���׸���� ���ΰ� �����ش�
								{
									player.sety(player.gety() - 12);
									player.seth(25);
								}
							}
							player.BitMove();
							//player.BitMove();
						}
					}
				}
			}
			else if (obj[i].getType() == 0)
			{

			}
			//if (ROWSPEED != 3)		//ROWSPEED�� ���Ƿ� �ٲ��־��ٸ� ���� ������ �ʱ�ȭ�� ������� �����ش�
			//	ROWSPEED = 3; ��� ���� �÷����� �ּ�ó���ϰ� ���� �ٵ� �̰� �����Ű�����

			//return;			//�ϳ��� �ε����ٸ� �״�� Ż���Ѵ�
		}
	}
	if (check_coll != 0)
		return;	//�ϳ��� �ε����ٸ� �״�� Ż��
	if ((player.getstate() == 4 || player.getstate() == 1)||(player.getstate()==5||player.getstate()==8))	//�ϳ��� ���ε������� ���߿��ִ°Ŵϱ� ���������Ѵ�
	{
		player.setstate(7);
		player.fall2save();		//�������� ������ x��ǥ�� ���
	}


}


//int(�� ��ȣ) �� ���� ��ֹ� ��ġ�� �־��ְ� ��� ������Ʈ�� ������ �˷��ִ� �Լ�
int initObject(OBJECT* obj, int mapnum, HINSTANCE g_hinst)
{
	int x, y, w, h, type;
	int objcount = 0;

	ifstream in;
	if (mapnum == 9)
	{
		in.open("map/map_0.txt", ios::in);
	}
	else if (mapnum == 10)
	{
		in.open("map/map_1.txt", ios::in);
	}
	else if (mapnum == 11)
	{
		in.open("map/map_2.txt", ios::in);
	}
	else if (mapnum == 12)
	{
		in.open("map/map_3.txt", ios::in);
	}
	else if (mapnum == 13)
	{
		in.open("map/map_4.txt", ios::in);
	}
	else {
		return 0;		//�� ���� �߸��ԷµǾ����� �״�� Ż��
	}




	if (in.is_open())
	{
		for (int i = 0; i < 150; ++i)
		{
			if (in.eof())
			{
				objcount = i;
				break;
			}
			in >> x >> y >> w >> h >> type;
			obj[i].create(x, y, w, h, type);
			obj[i].setHbit(g_hinst);

		}
	}
	return objcount;
}

//ī�޶� ������ũ
void adjustCamera(CAMERA& camera, PLAYER player)
{
	//�÷��̾��� �Ӹ��κ��� ī�޶��� ��������� �Ѿ�� �ٷ� ����ٰ��Ѵ�
	if (player.gety() - player.geth() < camera.GetY())
	{
		if (camera.GetY() <= 0)	//�ֻ����϶� �̵�����������
		{

		}
		else {
			camera.SetY(player.gety() - player.geth());		//384�� �� ũ�� 768�� ����
		}
	}
	else if (player.gety() + player.geth() > camera.GetY() + 768)	//ĳ������ �߹ٴ��� ī�޶���� �Ѿ��
	{
		if (camera.GetY() >= 3232)	//�������϶� �̵�����������
		{

		}
		else {
			camera.SetY(player.gety() + player.geth() - 768);	//�����ش�
		}
	}
	else if (camera.GetY() + 600 != player.gety())		//ī�޶� ������ ��ġ�� �����ʴٸ�
	{
		if (player.getstate() != 7)							//�׸��� ������������ ī�޶� �ٲ��ָ� �ʹ� ������ �̶��� ������
		{
			if (camera.GetY() <= 0)	//�ֻ����϶� �̵�����������
			{
				camera.SetY(0);
			}
			else if (camera.GetY() + 540 > player.gety())		//�󸶳� �ָ��ִ��Ŀ� ���� �ӵ��� �ٸ����ؼ� ī�޶� ��������Ѵ�
			{
				camera.SetY(camera.GetY() - 4);
			}
			else if (camera.GetY() + 580 > player.gety())
			{
				camera.SetY(camera.GetY() - 2);
			}
			else if (camera.GetY() + 600 > player.gety())
			{
				camera.SetY(camera.GetY() - 1);
			}
			else if (camera.GetY() + 700 < player.gety())
			{
				camera.SetY(camera.GetY() + 10);
			}
			else if (camera.GetY() + 680 < player.gety())
			{
				camera.SetY(camera.GetY() + 8);
			}
			else if (camera.GetY() + 660 < player.gety())
			{
				camera.SetY(camera.GetY() + 6);
			}
			else if (camera.GetY() + 640 < player.gety())
			{
				camera.SetY(camera.GetY() + 4);
			}
			else if (camera.GetY() + 620 < player.gety())
			{
				camera.SetY(camera.GetY() + 2);
			}
			else if (camera.GetY() + 600 < player.gety())
			{
				camera.SetY(camera.GetY() + 1);
			}
		}
	}


}
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
			if (player.GetPositionX() + player.GetHalfWidth() < object.getX()+object.getmx() || player.GetPositionX() - player.GetHalfWidth() > object.getX() + object.getmx() + object.getW()) return 0;
			if (player.GetPositionY() + player.GetHalfHeight() < object.getY() + object.getmy() || player.GetPositionY() - player.GetHalfHeight() > object.getY() + object.getmy() + object.getH()) return 0;
		}
		else
		{
			if (player.GetPositionX() + player.GetHalfWidth() < object.getX() || player.GetPositionX() - player.GetHalfWidth() > object.getX() + object.getW()) return 0;
			if (player.GetPositionY() + player.GetHalfHeight() < object.getY() || player.GetPositionY() - player.GetHalfHeight() > object.getY() + object.getH()) return 0;
		}
		

		return 1;
	} 
	else if (301 <= object.getType() && object.getType() < 401)	//����,���ٰ��� ���� ������ϴ� ������ ���δ� �÷��̾� �߱��� ��ƾ��ϰ� �Ʒ��δ� �÷��̾� �������� ������ ������ ���������� �־���ϹǷ� ���� ���ϰ� �ش�
	{

		if (player.GetPositionY() + player.GetHalfHeight() < object.getY() || player.GetPositionY() - player.GetHalfHeight() > object.getY() + object.getH()) return 0;	//�ϴܸ��� ������� ����

		if (object.getX() < player.GetPositionX() && player.GetPositionX() < object.getX() + object.getW())	//�������� �׷��� �� �β���� �̾ȿ������� cehck
		{
			if (UPkey == true)//����� Ư���ϰ� �ö󰡸� �ö󰡴��� üũ�� ���̳����Ѵ�.
			{

				if (player.GetPositionY() + player.GetHalfHeight() <= object.getY())	//�ö����� �Ʒ�Ű�� �����ϸ� �浹üũ x �ȱ׷��� �����ؼ� ��� �ٿ��Ŵ޸��� ����
					return 0;
				if (player.GetPositionY() < object.getY() + object.getH())
					return 1;
			}
			else if (DOWNkey == true)
			{

				if (player.GetPositionY() + player.GetHalfHeight() <= object.getY())
					return 1;
			}

			if (player.GetPositionY() + player.GetHalfHeight() <= object.getY() || player.GetPositionY() < object.getY() + object.getH())
				return 1;
		}
		
		return 0;
	}
	else if (object.getType() == 1)	//���ٴ��϶�
	{
		if (object.getX() <= player.GetPositionX() && player.GetPositionX() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.GetPositionY() + player.GetHalfHeight())
			{
				return 1;
			}
		}
	}
	else if (object.getType() <= 100) {	//�÷����϶��� �÷��̾� ������ ��Ʈ�ڽ����ȴ�
		if (object.getX() <= player.GetPositionX() && player.GetPositionX() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.GetPositionY() + player.GetHalfHeight() && player.GetPositionY() + player.GetHalfHeight() <= object.getY() + adjust)
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
	if (player.GetPositionX() - player.GetHalfWidth() < 0)
	{
		player.SetPositionX(player.GetHalfWidth());
		player.SetCMDMove(0);
	}
	else if (player.GetPositionX() + player.GetHalfWidth() > 1023)
	{
		player.SetPositionX(1023 - player.GetHalfWidth());
		player.SetCMDMove(0);
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

				if (player.GetState() == 7) //�������� ���϶� �ε����� ?
				{
					if (abs(player.GetSaveY() - player.GetPositionY()) > 200)	//�������޾ƾ��Ѵٸ�
					{
						if (player.GetInvincibleTime() == 0)	//������ �ƴ϶��
						{
							player.SetCMDMove(player.GetDirection());	//�����ִ¹������� ������ ������, �������µ� ���������� ������
							player.SetState(6);		//�ǰ����ΰ�
							player.TakeDamage(sound);
							return;
						}
					}
					player.SetPositionY(obj[i].getY() - player.GetHalfHeight());//�������� y��ǥ �������ְ�

					if (LEFTkey == 0 && RIGHTkey == 0)	//�ٵ� �׿��߿��� �ƹ�Ű�� �ȴ������־��� ? 
						player.SetCMDMove(false);	//�׷� ����������� ���°� ���ߵ������ش�.
					else if (LEFTkey == 1 && RIGHTkey == 1)
						player.SetCMDMove(false);	//���ÿ� �������־�� �����ش�
					else if (LEFTkey == 1)	//������ ������ �������־���?
						player.SetCMDMove(1);
					else if (RIGHTkey == 1)			//�׿�����ٲ��ش�
						player.SetCMDMove(2);

					if (DOWNkey == true) {
						player.SetState(3);	//���̰��ִ� ���¿��ٸ� ��� ���̰��־���
						player.SetPositionY(player.GetPositionY() + 12);
						player.SetHalfHeight(player.GetHalfHeight() - 12);	//����� �����ٸ� �ٽ� ���̱���·� ������
					}
					else player.SetState(1);				//���̴��� �ƴϿ����� �����ε������� ������������
					player.SetFallSpeed(0);			//�����������ӵ��� ���ѰŴ� �̰͵� ��������
					player.SetCMDHurt(0);			//���� ������� �ǰݾƴ�
					player.SetCMDRopeHurt(0);		//���� ������� �ǰݾƴ�

					if (ROWSPEED != 3)		//ROWSPEED�� ���Ƿ� �ٲ��־��ٸ� ���� ������ �ʱ�ȭ�� ������� �����ش�
						ROWSPEED = 3;
				}

				if (obj[i].getType() == 4)
				{
					player.SetPositionX(player.GetPositionX() + beltspeed);
				}
				if (obj[i].getType() == 6)
				{
					player.SetPositionX(player.GetPositionX() - beltspeed);
				}
			}
			else if (obj[i].getType() >= 101 && obj[i].getType() <= 200)	//��ֹ��� �ε�����
			{

				if (obj[i].getType() == 101)	//��ö��
				{
					if (player.GetInvincibleTime() == 0)	//������ �ƴ϶��
					{
						if (player.GetState() == 5 || player.GetState() == 8)
						{
							player.SetCMDRopeHurt(1);
						}
						if (player.GetState()==3) //���̰��־��ٸ�
						{
							player.SetPositionY(player.GetPositionY() - 12);
							player.SetHalfHeight(player.GetHalfHeight() + 12);	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (player.GetState() == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (player.GetCMDMove() == 1)
							{
								player.SetSpikeHurt(-8);	//8�� �������� ��
							}
							else if (player.GetCMDMove() == 2)
							{
								player.SetSpikeHurt(8);	//8�� ���������ΰ�
							}

							player.SetInvicible(100);	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							player.SetState(6);		//�ǰ����ΰ�
						}
						player.TakeDamage(sound);
					}
				}
				else if (obj[i].getType() == 102) //Break Pipe Left
				{
					//Copy and Paste is very good (Y Collapse)
					if (player.GetState() == 7) //�������� ���϶� �ε����� ?
					{
						player.SetPositionY(obj[i].getY() - player.GetHalfHeight());//�������� y��ǥ �������ְ�

						if (LEFTkey == 0 && RIGHTkey == 0)	//�ٵ� �׿��߿��� �ƹ�Ű�� �ȴ������־��� ? 
							player.SetCMDMove(false);	//�׷� ����������� ���°� ���ߵ������ش�.
						else if (LEFTkey == 1 && RIGHTkey == 1)
							player.SetCMDMove(false);	//���ÿ� �������־�� �����ش�
						else if (LEFTkey == 1)	//������ ������ �������־���?
							player.SetCMDMove(1);
						else if (RIGHTkey == 1)			//�׿�����ٲ��ش�
							player.SetCMDMove(2);

						player.SetState(1);				//�׸��� �����ε������� ������������
						player.SetFallSpeed(0);			//�����������ӵ��� ���ѰŴ� �̰͵� ��������
						player.SetCMDHurt(0);			//���� ������� �ǰݾƴ�
						player.SetCMDRopeHurt(0);		//���� ������� �ǰݾƴ�

						if (ROWSPEED != 3)		//ROWSPEED�� ���Ƿ� �ٲ��־��ٸ� ���� ������ �ʱ�ȭ�� ������� �����ش�
							ROWSPEED = 3;
					}
					//X Collapse
					if (player.GetState() == 1 || player.GetState() == 4) //Walking Collpse
					{
						if (obj[i].getY() < player.GetPositionY() - player.GetHalfHeight())
						{
							if (obj[i].getX() < player.GetPositionX() + player.GetHalfWidth()) //Left Collpse
							{
								player.SetPositionX(obj[i].getX() - player.GetHalfWidth());// x��ǥ �������ְ�
								player.SetCMDMove(0);//Don't Move
							}
						}
					}
				}
				else if (obj[i].getType() == 103) //���� ����, ���ÿ� ����� ��� ���Ⱑ ������ �վ���  �������� �ǰ������� �ִ�.
				{
					if (obj[i].getindex() == 2) //���Ⱑ ������ �վ��������� �ǰ��� �߻��Ѵ�
					{
						if (player.GetInvincibleTime() == 0)
						{
							if (player.GetState() == 5 || player.GetState() == 8)
							{
								player.SetCMDRopeHurt(1);
							}
							if (player.GetState() == 3) //���̰��־��ٸ�
							{
								player.SetPositionY(player.GetPositionY() - 12);
								player.SetHalfHeight(player.GetHalfHeight() + 12);	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
							}
							if (player.GetState() == 7)
							{
								if (player.GetDirection() == 1 || player.GetDirection() == 2) //������ �������ΰ�
								{
									player.SetSpikeHurt(-8);
								}

								player.SetInvicible(100);
							}
							else {
								player.SetCMDMove(1); //������ ������
								player.SetState(6);
							}
							player.TakeDamage(sound);
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
					if (player.GetInvincibleTime() == 0)	//������ �ƴ϶��
					{
						if (player.GetState() == 5 || player.GetState() == 8)
						{
							player.SetCMDRopeHurt(1);
						}
						if (player.GetState() == 3) //���̰��־��ٸ�
						{
							player.SetPositionY(player.GetPositionY() - 12);
							player.SetHalfHeight(player.GetHalfHeight() + 12);	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (player.GetState() == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (player.GetDirection() == 1)
							{
								player.SetSpikeHurt(-8);	//8�� �������� ��
							}
							else if (player.GetDirection() == 2)
							{
								player.SetSpikeHurt(8);	//8�� ���������ΰ�
							}

							player.SetInvicible(100);	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							player.SetCMDMove(player.GetDirection());
							player.SetState(6);		//�ǰ����ΰ�
						}
						player.TakeDamage(sound);
					}
				}
				else if (obj[i].getType() == 107)
				{
					if (player.GetInvincibleTime() == 0)	//������ �ƴ϶��
					{
						if (player.GetState() == 5 || player.GetState() == 8)
						{
							player.SetCMDRopeHurt(1);
						}
						if (player.GetState() == 3) //���̰��־��ٸ�
						{
							player.SetPositionY(player.GetPositionY() - 12);
							player.SetHalfHeight(player.GetHalfHeight() + 12);	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (player.GetState() == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (player.GetDirection() == 1)
							{
								player.SetSpikeHurt(-8);	//8�� �������� ��
							}
							else if (player.GetDirection() == 2)
							{
								player.SetSpikeHurt(8);	//8�� ���������ΰ�
							}

							player.SetInvicible(100);	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							player.SetCMDMove(player.GetDirection());
							player.SetState(6);		//�ǰ����ΰ�
						}
						player.TakeDamage(sound);
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
						player.Initialzie();
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
				if (player.GetJumpCooldown() <= 0)
				{
					if (player.IsRopeHurt() == 0)	//�������� ������ �ٽ� ���� ��Ž
					{
						if (UPkey == true || DOWNkey == true)
						{
							if (DOWNkey == true && (player.GetState() == 2 || player.GetState() == 7))	//���߿��ְų� �������϶� �Ʒ�Ű�δ� �ٿ� ����������
								return;

							if (player.GetState() != 5 && player.GetState() != 8)	//�ٿ� �Ŵ޷����� �ʾҴٸ� �ٿ� �Ŵ޸��� ���¸� ������ش�. �̹̺پ��ִٸ� �����ʿ����
							{
								player.SetState(5);
								if (UPkey == true)
									player.SetCMDMove(3);
								if (DOWNkey == true)
									player.SetCMDMove(4);
								player.SetPositionX(obj[i].getX() + (obj[i].getW() / 2));
								if (DOWNkey == true)	//�̶��� ���׸���ƴ϶� ���� �Ʒ��� �������°��̹Ƿ� ���׸���� ���ΰ� �����ش�
								{
									player.SetPositionY(player.GetPositionY() - 12);
									player.SetHalfHeight(25);
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
	if ((player.GetState() == 4 || player.GetState() == 1)||(player.GetState()==5||player.GetState()==8))	//�ϳ��� ���ε������� ���߿��ִ°Ŵϱ� ���������Ѵ�
	{
		player.SetState(7);
		player.SavePositionX();		//�������� ������ x��ǥ�� ���
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
	if (player.GetPositionY() - player.GetHalfHeight() < camera.GetY())
	{
		if (camera.GetY() <= 0)	//�ֻ����϶� �̵�����������
		{

		}
		else {
			camera.SetY(player.GetPositionY() - player.GetHalfHeight());		//384�� �� ũ�� 768�� ����
		}
	}
	else if (player.GetPositionY() + player.GetHalfHeight() > camera.GetY() + 768)	//ĳ������ �߹ٴ��� ī�޶���� �Ѿ��
	{
		if (camera.GetY() >= 3232)	//�������϶� �̵�����������
		{

		}
		else {
			camera.SetY(player.GetPositionY() + player.GetHalfHeight() - 768);	//�����ش�
		}
	}
	else if (camera.GetY() + 600 != player.GetPositionY())		//ī�޶� ������ ��ġ�� �����ʴٸ�
	{
		if (player.GetState() != 7)							//�׸��� ������������ ī�޶� �ٲ��ָ� �ʹ� ������ �̶��� ������
		{
			if (camera.GetY() <= 0)	//�ֻ����϶� �̵�����������
			{
				camera.SetY(0);
			}
			else if (camera.GetY() + 540 > player.GetPositionY())		//�󸶳� �ָ��ִ��Ŀ� ���� �ӵ��� �ٸ����ؼ� ī�޶� ��������Ѵ�
			{
				camera.SetY(camera.GetY() - 4);
			}
			else if (camera.GetY() + 580 > player.GetPositionY())
			{
				camera.SetY(camera.GetY() - 2);
			}
			else if (camera.GetY() + 600 > player.GetPositionY())
			{
				camera.SetY(camera.GetY() - 1);
			}
			else if (camera.GetY() + 700 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 10);
			}
			else if (camera.GetY() + 680 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 8);
			}
			else if (camera.GetY() + 660 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 6);
			}
			else if (camera.GetY() + 640 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 4);
			}
			else if (camera.GetY() + 620 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 2);
			}
			else if (camera.GetY() + 600 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 1);
			}
		}
	}


}
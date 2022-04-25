class Enemy
{
public :
	int x, y;
	Enemy()
	{
		x = 40;
		y = 7;
	}
	~Enemy()
	{

	}

	void enemyMove()
	{
		if (x >= 10)
		{
			x = x - 1;
		}
		else
		{
			x = 40;

		}
	}

};

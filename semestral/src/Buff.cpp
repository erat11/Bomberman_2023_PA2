class Buff
{
	public:
		Buff () {}
		virtual void takeEffect () {}
		virtual ~Buff() {}
	private:
		int duration;
		bool permanence;
		double dropChance;
};
class SineOscillator {

	float frequency;

	void initialize()
	{

	}

	int min_samples_per_period()
	{
		const float period = 1.0f / frequency;
		const float samples_per_period = 41000.0f / period; // (samples / second) / (seconds / period) = (samples / period)
	}
};

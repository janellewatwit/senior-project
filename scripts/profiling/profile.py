import statistics

f = open("C:\\Users\\felix\\dev\\school\\senior-project\\scripts\\profiling\\log.txt")

lines = f.readlines()

datapoints: dict[int, list[float]] = {}

for line in lines:
	split = line.split(":")
	num_voices: int = int(split[0])
	latency_ns: int = int(split[1])
	latency_ms: float = latency_ns / 1_000_000
	if not datapoints.get(num_voices):
		datapoints[num_voices] = []
	datapoints[num_voices].append(latency_ms)

for point, values in datapoints.items():
	print(str(point) + ": " + str(statistics.fmean(values)))

#include <StormByte/buffer/consumer.hxx>
#include <StormByte/buffer/pipeline.hxx>
#include <StormByte/buffer/producer.hxx>
#include <thread>

using namespace StormByte::Buffer;

void Pipeline::AddPipe(const PipeFunction& pipe) {
	m_pipes.push_back(pipe);
}

void Pipeline::AddPipe(PipeFunction&& pipe) {
	m_pipes.push_back(std::move(pipe));
}

void Pipeline::SetError() noexcept {
	for (auto& producer : m_producers) {
		producer.SetError();
	}
}

Consumer Pipeline::Process(Consumer buffer, const ExecutionMode& mode, std::shared_ptr<Logger> logger) noexcept {
	// Use pre-created producers corresponding to each pipe
	if (m_pipes.empty()) {
		return buffer; // no stages, passthrough
	}

	// Reset producers to ensure a fresh run when reusing the pipeline
	m_producers.clear();
	m_producers.resize(m_pipes.size());
	for (auto& prod : m_producers) {
		prod = Producer();
	}

	// Stage 0: consume from input, produce to m_producers[0]
	{
		Consumer stage_in = buffer;
		Producer& stage_out = m_producers[0];
		if (mode == ExecutionMode::Sync) {
			m_pipes[0](stage_in, stage_out, logger);
		} else {
			std::thread([pipe = m_pipes[0], in = stage_in, out = stage_out, logger]() mutable {
				pipe(in, out, logger);
			}).detach();
		}
	}

	// Remaining stages: consume from previous producer's consumer, output to own producer
	for (std::size_t i = 1; i < m_pipes.size(); ++i) {
		Consumer stage_in = m_producers[i - 1].Consumer();
		Producer& stage_out = m_producers[i];
		if (mode == ExecutionMode::Sync) {
			m_pipes[i](stage_in, stage_out, logger);
		} else {
			std::thread([pipe = m_pipes[i], in = stage_in, out = stage_out, logger]() mutable {
				pipe(in, out, logger);
			}).detach();
		}
	}

	return m_producers.back().Consumer();
}
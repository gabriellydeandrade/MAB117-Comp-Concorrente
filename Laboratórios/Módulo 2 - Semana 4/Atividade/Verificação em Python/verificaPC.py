class PC:
	def __init__(self):
		self.tamanho = 10
		self.buffer = [0] * self.tamanho
		self.posicao_consumidor = -1
		self.posicao_produtor = -1

	def buffer_vazio(self):
		buffer_vazio = True
		for elemento in self.buffer:
			if elemento:
				buffer_vazio = False
				break

		return buffer_vazio

	def consumidorBloqueado(self, id):
		if self.posicao_consumidor != -1 or not self.buffer_vazio():
			print(f"ERRO: consumidor {id} bloqueado quando há conteúdo no buffer")


	def consumidorPegou(self, id, valor):
		valor_esperado = self.buffer[self.posicao_consumidor]

		if valor != valor_esperado:
			print(f"ERRO: consumidor {id} pegou o valor não esperado. Recebido: {valor}, Esperado: {valor_esperado}")

		self.buffer[self.posicao_consumidor] = False # Marreta valor que já foi pego

		self.posicao_consumidor = (self.posicao_consumidor + 1) % self.tamanho

		if (self.posicao_consumidor == self.posicao_produtor):
			self.posicao_consumidor = self.posicao_produtor = -1


	def produtorBloqueado(self, id):
		if self.posicao_produtor != self.posicao_consumidor:
			print(f"ERRO: produtor {id} bloqueado quando buffer não está cheio")

		if self.buffer_vazio():
			print(f"ERRO: produtor {id} bloqueado quando buffer está vazio")

	def produtorInseriu(self, id, valor):
		if self.posicao_consumidor == -1:
			self.posicao_consumidor = self.posicao_produtor = 0

		self.buffer[self.posicao_produtor] = valor
		self.posicao_produtor = (self.posicao_produtor + 1) % self.tamanho

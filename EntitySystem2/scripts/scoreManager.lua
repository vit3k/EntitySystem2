local scoreManager = {
	score = { 0, 0 },

	--Collision handler for scoring surfaces
	--Adds point for player and attach ball to the other
	onScoreCollision = function(self, event)
		local scoringSurface

		if event.entity1:has(Components.ScoringSurface) then
			scoringSurface = event.entity1
		elseif event.entity2:has(Components.ScoringSurface) then
			scoringSurface = event.entity2
		end

		if scoringSurface ~= nil then
			local scoring = scoringSurface:get(Components.ScoringSurface)
			self.score[scoring.playerId + 1] = self.score[scoring.playerId + 1] + 1
			self.scoreText:get(Components.Text).text = self.score[1] .. " - " .. self.score[2]

			local paddleTransform = scoring.paddle:get(Components.Transform)

			self.ball:attach(Components.Attach, {
				parentTransform = paddleTransform,
				relativePosition = { x = glm.normalize(paddleTransform.position).x * (-1), y = 0.75 },
				paddle = scoring.paddle
			})

			local ballVelocity = self.ball:get(Components.Velocity)
			print(ballVelocity.velocity.x .. ' ' .. ballVelocity.velocity.y)
			ballVelocity.velocity.x = scoring.launchVelocity;
			ballVelocity.velocity.y = 0;

			if self.score[1] >= 3 or self.score[2] >= 3 then
				emit(Events.GameOver, { score = self.score })
			end
		end
	end,

	init = function(self, scoreText, ball)
		self.scoreText = scoreText
		self.ball = ball
		subscribeForObject(Events.Collision, self.onScoreCollision, self)
	end
}

return scoreManager
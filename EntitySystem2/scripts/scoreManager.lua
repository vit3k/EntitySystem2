local score = { 0, 0 }

--Collision hanlder for scoring surfaces
--Adds point for player and attach ball to the other
function onScoreCollision(event)
	local scoringSurface
	if event.entity1:has(Components.ScoringSurface) then
		scoringSurface = event.entity1
	elseif event.entity2:has(Components.ScoringSurface) then
		scoringSurface = event.entity2
	end

	if scoringSurface ~= nil then
		scoring = scoringSurface:get(Components.ScoringSurface)
		score[scoring.playerId + 1] = score[scoring.playerId + 1] + 1
		scoreText:get(Components.Text).text = score[1] .. " - " .. score[2]

		paddleTransform = scoring.paddle:get(Components.Transform)
		ball:attach(Components.Attach, {
			parentTransform = paddleTransform,
			relativePosition = { x = glm.normalize(paddleTransform.position).x * (-1), y = 0.75 }
		})
		
	end
end

function init(scoreText, ball)
	scoreText = scoreText
	ball = ball
	subscribe(Events.Collision, onScoreCollision)
end

return {
	init = init
}

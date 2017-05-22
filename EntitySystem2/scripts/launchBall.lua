return {
  onLaunchBall = function(self, event)
    if self.ball:has(Components.Attach) then
      
      local attach = self.ball:get(Components.Attach)
      local paddleInput = attach.paddle:get(Components.Input)

      if paddleInput.controller == event.controller then
        local paddleVelocity = attach.paddle:get(Components.Velocity)
        print("launchball")
        self.ball:detach(Components.Attach)
        local ballVelocity = self.ball:get(Components.Velocity)
        print(paddleVelocity)
        ballVelocity.velocity.y = glm.clamp(paddleVelocity.velocity.y, -6, 6)
      end
    end
  end,

  init = function(self, ball)
    self.ball = ball
    subscribeForObject(Events.LaunchBall, self.onLaunchBall, self)
  end
}
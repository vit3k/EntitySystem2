return {
  onLaunchBall = function(self, event)
    if self.ball:has(Components.Attach) then
      print("launchball")
      self.ball:detach(Components.Attach)
      local velocity = self.ball:get(Components.Velocity)
      print(velocity.velocity.x.. ' ' .. velocity.velocity.y)
    end
  end,

  init = function(self, ball)
    self.ball = ball
    subscribeForObject(Events.LaunchBall, self.onLaunchBall, self)
  end
}
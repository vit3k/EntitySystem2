return {
  onScore = function(self, event)
    print("scored")
    if event[1] >= 2 then
      --player 1 won
      print("player 1 wins")
    elseif event[2] >= 2 then
      --player 2 won
      print("player 2 wins")
    end
  end,

  init = function(self)
    subscribeForObject(Events.Scored, self.onScore, self)
  end
}
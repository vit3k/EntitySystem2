local table = require('level')
local score = require('score')

return {
  state = table,

  onGameOver = function(self, event)
    print('onGameOver')
    self.state = score
    self.state:init(event.score)
  end,

  init = function(self)
    subscribeForObject(Events.GameOver, self.onGameOver, self)
    self.state:init()
  end
}
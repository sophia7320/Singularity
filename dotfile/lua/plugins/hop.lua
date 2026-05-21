return {
	"smoka7/hop.nvim",
	opts = {
		hint_position = 3,
		--这个是硬编码
		enable = false;
	},
	keys = {
		{ "<leader>hp", "<Cmd>HopWord<CR>", desc = "hop word" , silent =true}
	}

	--[[
	--config = function()
		--require("hop").setup
	--]]
}

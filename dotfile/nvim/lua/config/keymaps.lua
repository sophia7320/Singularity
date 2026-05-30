local map = vim.keymap

map.set("n", "<leader>w", "<cmd>w<CR>", { desc = "save the file" })
map.set("n", "<leader>q", "<cmd>wq<CR>", { desc = "quit" })

map.set("i", "<Tab>", function()
	if vim.fn.pumvisible() == 1 then
		return "<C-n>"
	else
		return "<Tab>"
	end
end, { desc = "auto Tab", expr = true })

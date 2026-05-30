vim.lsp.config("lua_ls", {
	filetypes = { "lua" },

	settings = {
		Lua = {
			diagnostics = {
				globals = { "vim" },
			},
		},
	},
})

local capabilities = require("blink.cmp").get_lsp_capabilities({
	textDocument = {
		completion = {
			snippetSupport = true,
		},
	},
}, true)

vim.lsp.config("*", {
	capabilities = capabilities,
})
